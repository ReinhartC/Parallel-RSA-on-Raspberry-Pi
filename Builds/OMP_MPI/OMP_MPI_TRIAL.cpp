#include <stdlib.h>
#include <math.h>
#include <limits.h>
#include <iostream>
#include <fstream>
#include <sstream>
#include <string.h>
#include <time.h>
#include <omp.h>
#include <mpi.h>

#define MAX_STR_LEN 2000
#define MAX_LINE 10000

#define CHUNK 1
#define NUM_PI 3
#define NUM_THREADS 4
#define MASTER 0

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

   long long int pube, pubmod, prive, privmod;
   size_t len[MAX_LINE];

   // Process command input
   short int command=atoi(mpinput[1]);

   // Input files
   char* input_key_path = mpinput[2];
   char* input_file_path = mpinput[3];

   // Line amount
   int line = atoi(mpinput[4]);

   // Processing Variables
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
      case 0: //Encrypt
      {
         if(rank == MASTER){
            // Public key load
            std::ifstream pubkey(input_key_path);
            pubkey >> pube >> pubmod;
            pubkey.close();      
         }

         // Broadcasting key to nodes
         MPI_Bcast(&pube, 1, MPI_LONG_LONG, MASTER, MPI_COMM_WORLD);
         MPI_Bcast(&pubmod, 1, MPI_LONG_LONG, MASTER, MPI_COMM_WORLD);
         
         // Synchronisation
         MPI_Barrier(MPI_COMM_WORLD);   

         // Amount of line to be worked by a node
         int work = line/NUM_PI;

         // Work start and end point for each nodes
         int startline = rank*work;
         int endline = (rank+1)*work;

         // Temp processed filename
         std::stringstream tempfile;
         tempfile << "enc" << rank;

         // Plaintext load, encryption, and ciphertext writing to output file
         std::ifstream plaintext(input_file_path);
         std::ofstream encrypted(tempfile.str());

         // Plaintext encryption loop
         for(int i=startline; i<endline; i++){
            if(!plaintext.eof()){
               plaintext.getline(inmsg,MAX_STR_LEN);
               len = strlen(inmsg);
               char2longlong(inmsg, inmsg_ll);

               encrypt(inmsg_ll, pube, pubmod, outmsg_ll, len);

               for(int j=0; j<len; j++)
                  encrypted << outmsg_ll[j] << " ";
               encrypted << 0 << std::endl;
            }
         }

         plaintext.close();
         encrypted.close();

         // Printing
         if(rank==MASTER){
            std::ofstream merge("decrypted.txt");
            for(int i=0; i<3; i++){
               std::stringstream tempprint;
               tempprint << "dec" << i;

               std::ofstream print(tempprint.str());
               while(!print.eof()){
                  while()
                     encrypted << outmsg_ll[j] << " ";
                  encrypted << 0 << std::endl;
               }
               print.close();
               remove(tempprint.str());
            }
            merge.close();
         }

         // Final synchronisation and finalizing
         MPI_Barrier(MPI_COMM_WORLD);
         MPI_Finalize();

         if(rank == MASTER)
         	std::cout << std::endl << "'" << input_file_path << "'" << " file encryption done" << std::endl;
         break;
      }

      case 1: //Decrypt
      {
         if(rank == MASTER){
            // Private key load
            std::ifstream privkey(input_key_path);
            privkey >> prive >> privmod;
            privkey.close();      
         }

         // Broadcasting key to nodes
         MPI_Bcast(&prive, 1, MPI_LONG_LONG, MASTER, MPI_COMM_WORLD);
         MPI_Bcast(&privmod, 1, MPI_LONG_LONG, MASTER, MPI_COMM_WORLD);
         
         // Synchronisation
         MPI_Barrier(MPI_COMM_WORLD);

         // Temp processed filename
         std::stringstream tempfile;
         tempfile << "dec" << rank;

         // Amount of line to be worked by a node
         int work = line/NUM_PI;

         // Work start and end point for each nodes
         int startline = rank*work;
         int endline = (rank+1)*work;

         // Ciphertext load, decryption, and writing results to output file
         std::ifstream ciphertext(input_file_path);
         std::ofstream decrypted(tempfile.str());

         // Ciphertext decryption loop
         for(int i=startline; i<endline; i++){
         	while(ciphertext >> inmsg_ll[len]) {
               if(inmsg_ll[len]==0) break;
               len++;
            }
            
            decrypt(inmsg_ll, prive, privmod, decrmsg_ll, len);
            longlong2char(decrmsg_ll, decrmsg);
            decrypted << decrmsg << std::endl;
            len=0;
         }

         decrypted.close();
         ciphertext.close();

         // Printing
         if(rank==MASTER){
            std::ofstream merge("decrypted.txt");
            for(int i=0; i<3; i++){
               std::stringstream tempprint;
               tempprint << "dec" << i;

               std::ofstream print(tempprint.str());
               while(!print.eof()){
                  plaintext.getline(printmsg,MAX_STR_LEN);
                  merge << printmsg << std::endl;
               }
               print.close();
               remove(tempprint.str());
            }
            merge.close();
         }
         
         // Final synchronisation and finalizing
         MPI_Barrier(MPI_COMM_WORLD);
         MPI_Finalize();

         if(rank == MASTER)
         	std::cout << std::endl << "'" << input_file_path << "'" << " file decryption done" << std::endl;
         break;
      }
   }

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
