#include <stdlib.h>
#include <math.h>
#include <limits.h>
#include <iostream>
#include <fstream>
#include <string.h>
#include <time.h>
#include <omp.h>
#include <mpi.h>

#define MAX_STR_LEN 10000

#define CHUNK 1
#define NUM_PI 3
#define NUM_THREADS 4
#define MASTER 0

long long int prime(long long int);
long long int gcd(long long int p, long long int q);
int publickey(long long int p, long long int q, long long int* exp, long long int* mod);
int privatekey(long long int p, long long int q, long long int pubexp, long long int* exp, long long int* mod);
int encrypt(long long int* inmsg, long long int, long long int, long long int* outmsg, size_t len);
int decrypt(long long int* inmsg, long long int, long long int, long long int* outmsg, size_t len);
int char2longlong(char* in, long long int* out);
int longlong2char(long long int* in, char* out);

// MPI Variables
int size, rank;

int main(int mpinit, char** mpinput) {

   // Setup MPI
   MPI_Init(&mpinit, &mpinput);
   MPI_Comm_size(MPI_COMM_WORLD, &size);
   MPI_Comm_rank(MPI_COMM_WORLD, &rank);

   short int pi=0;

   long long int p,q, pube, pubmod, prive, privmod;
   size_t len=0;

   // Process command input
   short int command=atoi(mpinput[1]);

   // Input files
   char* input_key_path = mpinput[2];
   char* input_file_path = mpinput[3];

   // Line amount
   int line = atoi(mpinput[4]);

   char inmsg[MAX_STR_LEN];
   long long int inmsg_ll[MAX_STR_LEN];
   char outmsg[MAX_STR_LEN];
   long long int outmsg_ll[MAX_STR_LEN];
   char decrmsg[MAX_STR_LEN];
   long long int decrmsg_ll[MAX_STR_LEN];

   char node_name[MPI_MAX_PROCESSOR_NAME];
   int name_len;
   MPI_Get_processor_name(node_name, &name_len);

   if (rank == MASTER)
      std::cout << "Parallel RSA with OpenMP and MPI"<< std::endl << std::endl;
   
   // Show running nodes
   std::cout << "Running on " << node_name << std::endl;

   switch(command){
      case 0: //Generate Keys
      {
         // Generate prime numbers (p and q)
         srand (time(NULL));

         p=rand() % 100+1;
         while(prime(p)!=0) p++;

         q=p+rand() % 1000+1;
         while(prime(q)!=0 || q==p) q++;

        // Generating public and private key
         publickey(p,q,&pube,&pubmod);
         privatekey(p,q,pube,&prive,&privmod);

         // Writing public and private key to a file
         std::ofstream pubkey("pubkey.txt");
         pubkey << pube << " " << pubmod;
         pubkey.close();

         std::ofstream privkey("privkey.txt");
         privkey << prive << " " << privmod;
         privkey.close();

         std::cout << "Public key and Private key has been created on " << node_name << std::endl;
         break;
      }

      case 1: //Encrypt
      {
         if(rank == MASTER){
            // Public key load
            std::ifstream pubkey(input_key_path);
            while(!pubkey.eof()){
               pubkey >> pube >> pubmod;
            }
            pubkey.close();      
         }

         // Broadcasting key to nodes
         MPI_Bcast(&pube, 1, MPI_LONG_LONG, MASTER, MPI_COMM_WORLD);
         MPI_Bcast(&pubmod, 1, MPI_LONG_LONG, MASTER, MPI_COMM_WORLD);
         
         // Synchronisation
         MPI_Barrier(MPI_COMM_WORLD);   

         std::stringstream ss;
         ss << "enc" << rank;
         std::string tempfile = ss.str();

         // Plaintext load, encryption, and ciphertext writing to output file
         std::ifstream plaintext(input_file_path);
         std::ofstream encrypted(tempfile);

         // Plaintext encryption loop
         int work=line/NUM_PI;
         for(int i=rank; i<line; i+=NUM_PI){
            plaintext.getline(inmsg,MAX_STR_LEN);
            len = strlen(inmsg);
            char2longlong(inmsg, inmsg_ll);

            encrypt(inmsg_ll, pube, pubmod, outmsg_ll, len);

            for(int j=0; j<len; j++)
               encrypted << outmsg_ll[j] << " ";
            encrypted << 0 << std::endl;
         }

         plaintext.close();
         encrypted.close();

         // Final synchronisation and finalizing
         MPI_Barrier(MPI_COMM_WORLD);
         MPI_Finalize();

         if(rank == MASTER)
         	std::cout << std::endl << "'" << input_file_path << "'" << " file encryption done" << std::endl;
         break;
      }

      case 2: //Decrypt
      {
         if(rank == MASTER){
            // Private key load
            std::ifstream privkey(input_key_path);
            while(!privkey.eof()){
               privkey >> prive >> privmod;
            }
            privkey.close();      
         }

         // Broadcasting key to nodes
         MPI_Bcast(&prive, 1, MPI_LONG_LONG, MASTER, MPI_COMM_WORLD);
         MPI_Bcast(&privmod, 1, MPI_LONG_LONG, MASTER, MPI_COMM_WORLD);
         
         // Synchronisation
         MPI_Barrier(MPI_COMM_WORLD);

         // Ciphertext load, decryption, and writing results to output file
         std::ifstream ciphertext(input_file_path);

         if(rank == MASTER){
            std::ofstream dec0("dec0");
            // Ciphertext decryption loop
            int work=line/NUM_PI;
            for(int i=rank; i<line; i+=NUM_PI){
            	while(ciphertext >> inmsg_ll[len]) {
                  if(inmsg_ll[len]==0) break;
                  len++;
               }
               
               decrypt(inmsg_ll, prive, privmod, decrmsg_ll, len);
               longlong2char(decrmsg_ll, decrmsg);
               dec0 << decrmsg << std::endl;
               len=0;
            }
            dec0.close();
         }
         else if(rank == 1){
            std::ofstream dec1("dec1");
            // Ciphertext decryption loop
            int work=line/NUM_PI;
            for(int i=rank; i<line; i+=NUM_PI){
               while(ciphertext >> inmsg_ll[len]) {
                  if(inmsg_ll[len]==0) break;
                  len++;
               }
               
               decrypt(inmsg_ll, prive, privmod, decrmsg_ll, len);
               longlong2char(decrmsg_ll, decrmsg);
               dec1 << decrmsg << std::endl;
               len=0;
            }
            dec1.close();
         }
         else if(rank == 2){
            std::ofstream dec2("dec2");
            // Ciphertext decryption loop
            int work=line/NUM_PI;
            for(int i=rank; i<line; i+=NUM_PI){
               while(ciphertext >> inmsg_ll[len]) {
                  if(inmsg_ll[len]==0) break;
                  len++;
               }
               
               decrypt(inmsg_ll, prive, privmod, decrmsg_ll, len);
               longlong2char(decrmsg_ll, decrmsg);
               dec2 << decrmsg << std::endl;
               len=0;
            }
            dec2.close();
         }
         ciphertext.close();
         

         // Final synchronisation and finalizing
         MPI_Barrier(MPI_COMM_WORLD);
         MPI_Finalize();

         if(rank == MASTER)
         	std::cout << std::endl << "'" << input_file_path << "'" << " file decryption done" << std::endl;
         break;
      }
   }

}

// Prime check, returns 0 if prime
long long int prime(long long int p){
   long long int j = sqrt(p);
   for (long long int z=2;z<j;z++)
      if (0==p%z) return z;
   return 0;
}

// Public key generate functions
int publickey(long long int p, long long int q, long long int *exp, long long int *mod){
   *mod = (p-1)*(q-1);
   *exp = (int)sqrt(*mod);

   while (1!=gcd(*exp,*mod)) (*exp)++;
   *mod = p*q;

   return 0;
}

// Private key generate functions
int privatekey(long long int p, long long int q, long long int pubexp, long long int *exp, long long int *mod){
   *mod = (p-1)*(q-1);
   *exp = 1;

   long long int tmp=pubexp;

   while(1!=tmp%*mod){
      tmp+=pubexp;
      tmp%=*mod;
      (*exp)++;
   }
   *mod = p*q;

   return 0;
}

// Encryption function
int encrypt(long long int* in, long long int exp, long long int mod, long long int* out, size_t len){
   #pragma omp parallel for default(shared) schedule(dynamic, CHUNK) num_threads(NUM_THREADS)
   for (int i=0; i < len; i++){
      long long int c = in[i];
      for (int z=1;z<exp;z++){
         c *= in[i];
         c %= mod;
      }
      out[i] = c;
   }
   out[len]='\0';
   return 0;
}

// Decryption function
int decrypt(long long int* in, long long int exp, long long int mod, long long int* out, size_t len){
   #pragma omp parallel for default(shared) schedule(dynamic, CHUNK) num_threads(NUM_THREADS)
   for (int i=0; i < len; i++){
      long long int c = in[i];

      for (int z=1;z<exp;z++){
         c *= in[i];
         c %= mod;
      }
      out[i] = c;
   }
   out[len]='\0';

   return 0;
}

// Greatest common divisor calculation
long long int gcd(long long int p, long long int q){
   if (p<q){
      long long int tmp=p;
      p=q;
      q=tmp;
   }

   while (q!=0){
      long long int tmp = q;
      q = p%q;
      p = tmp;
   }

   return p;
}

// Long long integer converter
int longlong2char(long long int* in, char* out){
   while(*in != 0) *out++ = (char)(*in++);
   *out = '\0';

   return 0;
}

// Character to long long integer converter
int char2longlong(char* in, long long int* out){
   while(*in != '\0') *out++ = (long long int)(*in++);
   *out = 0;

   return 0;
}
