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
#define NUM_PI_SLAVE 1
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
   
   short int pi=1;

   long long int p,q, pube, pubmod, prive, privmod;
   size_t len=0;
   
   // Process command input
   short int command=atoi(mpinput[1]);
   
   // Input files
   char* input_key_path = mpinput[2];
   char* input_file_path = mpinput[3];

   char inmsg[MAX_STR_LEN];
   long long int inmsg_ll[MAX_STR_LEN];
   char outmsg[MAX_STR_LEN];
   long long int outmsg_ll[MAX_STR_LEN];
   char decrmsg[MAX_STR_LEN];
   long long int decrmsg_ll[MAX_STR_LEN];
   
   char node_name[MPI_MAX_PROCESSOR_NAME];
   int name_len;
   MPI_Get_processor_name(node_name, &name_len);
   std::cout << "Parallel RSA running on " << node_name << std::endl;

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
         //Public key load
      	std::ifstream pubkey(input_key_path);
         while(!pubkey.eof()){
            pubkey >> pube >> pubmod;
         }
         pubkey.close();
         
         // Broadcasting key to nodes
         MPI_Bcast(&pube, 1, MPI_LONG_LONG, MASTER, MPI_COMM_WORLD);
         MPI_Bcast(&pubmod, 1, MPI_LONG_LONG, MASTER, MPI_COMM_WORLD);
         
         // Synchronisation
         MPI_Barrier(MPI_COMM_WORLD);

         // Plaintext load, encryption, and ciphertext writing to output file
         std::ifstream plaintext(input_file_path);
         std::ofstream encrypted("encrypted.txt");         
         // Plaintext encryption loop
         while(!plaintext.eof()){
            plaintext.getline(inmsg,MAX_STR_LEN);
            len = strlen(inmsg);         			
            char2longlong(inmsg, inmsg_ll);

			   // Sending data to each slave
            if(rank == MASTER){ 
               MPI_Send(inmsg_ll, 1, MPI_LONG_LONG, pi, 0 , MPI_COMM_WORLD);
               MPI_Send(&pube, 1, MPI_LONG_LONG, pi, 0 , MPI_COMM_WORLD);
               MPI_Send(&pubmod, 1, MPI_LONG_LONG, pi, 0 , MPI_COMM_WORLD);
               MPI_Send(&len, 1, MPI_LONG_LONG, pi, 0 , MPI_COMM_WORLD);
            }
            else{
               MPI_Recv(inmsg_ll, 1, MPI_LONG_LONG, MASTER, 0, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
               MPI_Recv(&pube, 1, MPI_LONG_LONG, MASTER, 0, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
               MPI_Recv(&pubmod, 1, MPI_LONG_LONG, MASTER, 0, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
               MPI_Recv(&len, 1, MPI_LONG_LONG, MASTER, 0, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
            }

            // Node rotation
            if(pi<NUM_PI_SLAVE) pi++;
            else pi=1;

            encrypt(inmsg_ll, pube, pubmod, outmsg_ll, len);

            if(rank != MASTER)
               MPI_Send(outmsg_ll, 1, MPI_LONG_LONG, MASTER, 0 , MPI_COMM_WORLD);
            else
               MPI_Recv(outmsg_ll, 1, MPI_LONG_LONG, pi, 0, MPI_COMM_WORLD, MPI_STATUS_IGNORE);		    

            for(int i=0; i<len; i++) 
               encrypted << outmsg_ll[i] << " ";
            encrypted << 0 << std::endl;                          
         }		 	 
         plaintext.close();                
         encrypted.close();

		   // Final synchronisation and finalizing
         MPI_Barrier(MPI_COMM_WORLD);
         MPI_Finalize();

         std::cout << "Encryption done from " << node_name << std::endl;
         break;
      }

      case 2: //Decrypt
      {  
	  	   // Private key load
         std::ifstream privkey(input_key_path);
         while(!privkey.eof()){
            privkey >> prive >> privmod;
         }
         privkey.close();

         // Broadcasting key to nodes
         MPI_Bcast(&prive, 1, MPI_LONG_LONG, MASTER, MPI_COMM_WORLD);
         MPI_Bcast(&privmod, 1, MPI_LONG_LONG, MASTER, MPI_COMM_WORLD);

         // Synchronisation
         MPI_Barrier(MPI_COMM_WORLD);

		   // Ciphertext load, decryption, and writing results to output file
         std::ifstream ciphertext(input_file_path);
         std::ofstream decrypted("decrypted.txt");
         // Ciphertext decryption loop
         while(!ciphertext.eof()){
            while(ciphertext >> inmsg_ll[len]) {
               if(inmsg_ll[len]==0) break;
               len++;
            }

			   // Sending data to each slave
            if(rank == MASTER){ 
               MPI_Send(inmsg_ll, 1, MPI_LONG_LONG, pi, 0 , MPI_COMM_WORLD);
               MPI_Send(&prive, 1, MPI_LONG_LONG, pi, 0 , MPI_COMM_WORLD);
               MPI_Send(&privmod, 1, MPI_LONG_LONG, pi, 0 , MPI_COMM_WORLD);
               MPI_Send(&len, 1, MPI_LONG_LONG, pi, 0 , MPI_COMM_WORLD);
            }
            else{
               MPI_Recv(inmsg_ll, 1, MPI_LONG_LONG, MASTER, 0, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
               MPI_Recv(&prive, 1, MPI_LONG_LONG, MASTER, 0, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
               MPI_Recv(&privmod, 1, MPI_LONG_LONG, MASTER, 0, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
               MPI_Recv(&len, 1, MPI_LONG_LONG, MASTER, 0, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
            }

            if(pi<NUM_PI_SLAVE) pi++;
            else pi=1;

            decrypt(inmsg_ll, prive, privmod, decrmsg_ll, len);

            if(rank != MASTER)
               MPI_Send(outmsg_ll, 1, MPI_LONG_LONG, MASTER, 0 , MPI_COMM_WORLD);
            else
               MPI_Recv(outmsg_ll, 1, MPI_LONG_LONG, pi, 0, MPI_COMM_WORLD, MPI_STATUS_IGNORE);

            longlong2char(decrmsg_ll, decrmsg);
            decrypted << decrmsg << std::endl;
            len=0;
         }                                              
         ciphertext.close(); 
         decrypted.close();

         // Final synchronisation and finalizing
         MPI_Barrier(MPI_COMM_WORLD);
         MPI_Finalize();

         std::cout << "Decryption done from " << node_name << std::endl;
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
