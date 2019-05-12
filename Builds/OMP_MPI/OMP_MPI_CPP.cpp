#include <stdlib.h>
#include <math.h>
#include <limits.h>
#include <iostream>
#include <fstream>
#include <string.h>
#include <time.h>
#include <omp.h>
#include <mpi.h>

#define MAX_STR_LEN 2000
#define MAX_LINE 5000

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

int main(int argc, char** argv) {

   // Setup MPI
   MPI::Init(argc, argv);
   size = MPI::COMM_WORLD.Get_size();
   rank = MPI::COMM_WORLD.Get_rank();

   long long int pube, pubmod, prive, privmod;
   size_t len[MAX_LINE];

   // Process command input
   short int command=atoi(argv[1]);

   // Input files
   char* input_key_path = argv[2];
   char* input_file_path = argv[3];

   // Line amount
   int line = atoi(argv[4]);

   // Processing Variables
   char inmsg[MAX_LINE][MAX_STR_LEN];
   long long int inmsg_ll[MAX_LINE][MAX_STR_LEN];
   char outmsg[MAX_LINE][MAX_STR_LEN];
   long long int outmsg_ll[MAX_LINE][MAX_STR_LEN];

   //char node_name[MPI::MAX_PROCESSOR_NAME];
   //int name_len;
   //MPI_Get_processor_name(node_name, &name_len);

   if (rank == MASTER)
      std::cout << "Parallel RSA with OpenMP and MPI"<< std::endl << std::endl;
   
   // Show running nodes
   //std::cout << "Running on " << node_name << std::endl;

   switch(command){
      case 0: //Encrypt
      {
         if(rank == MASTER){
            // Public key load
            std::ifstream pubkey(input_key_path);
            pubkey >> pube >> pubmod;
            pubkey.close();

            // Plaintext load
            std::ifstream plaintext(input_file_path);
            for(int i=0; !plaintext.eof(); i++){
               plaintext.getline(inmsg[i],MAX_STR_LEN);
               len[i] = strlen(inmsg[i]);
            }
            plaintext.close();
         }

         // Broadcasting key to nodes
         MPI::COMM_WORLD.Bcast(&pube, 1, MPI::LONG_LONG, MASTER);
         MPI::COMM_WORLD.Bcast(&pubmod, 1, MPI::LONG_LONG, MASTER);
         MPI::COMM_WORLD.Bcast(len, line, MPI::UNSIGNED, MASTER);

         // Synchronisation after broadcasting
         MPI::COMM_WORLD.Barrier();

         // Amount of line to be worked by a node
         int work = line/NUM_PI;
         int sendsize = work*MAX_STR_LEN;

         // Distributing plaintext to nodes
         if(rank == MASTER){
            MPI::COMM_WORLD.Send(inmsg[0]+sendsize, sendsize, MPI::CHAR, 1, 0);
            MPI::COMM_WORLD.Send(inmsg[0]+2*sendsize, sendsize, MPI::CHAR, 2, 0);
         }
         if(rank != MASTER)
            MPI::COMM_WORLD.Recv(inmsg[0], sendsize, MPI::CHAR, MASTER, 0);

         // Start point for each nodes
         int startline = rank*work;

         // Plaintext encryption loop
         for(int i=0; i<work; i++){
            char2longlong(inmsg[i], inmsg_ll[i]);
            encrypt(inmsg_ll[i], pube, pubmod, outmsg_ll[i], len[i+startline]);
         }

         // Collecting encrypted text back to master node
         if(rank != MASTER)
            MPI::COMM_WORLD.Send(outmsg_ll[0], sendsize, MPI::LONG_LONG, MASTER, 0);
         if(rank == MASTER){
            MPI::COMM_WORLD.Recv(outmsg_ll[0]+sendsize, sendsize, MPI::LONG_LONG, 1, 0);
            MPI::COMM_WORLD.Recv(outmsg_ll[0]+2*sendsize, sendsize, MPI::LONG_LONG, 2, 0);
         }

         // Synchronisation before printing
         MPI::COMM_WORLD.Barrier();

         if(rank == MASTER){
            std::ofstream encrypted("encrypted.txt");
            for(int i=0; i<line; i++){
               for(auto print : outmsg_ll[i]){
                  encrypted << print << " ";
                  if(print==0) break;
               }
               encrypted << std::endl;
            }
            encrypted.close();
         }

         // Final synchronisation and finalizing
         MPI::COMM_WORLD.Barrier();
         MPI::Finalize();

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

            // Ciphertext load
            std::ifstream ciphertext(input_file_path);
            for(int i=0; !ciphertext.eof(); i++){
               len[i]=0;
               while(ciphertext >> inmsg_ll[i][len[i]]) {
                  len[i]++;
                  if(inmsg_ll[i][len[i]]==0) break;
               }
            }
            ciphertext.close();
         }

         // Broadcasting key to nodes
         MPI::COMM_WORLD.Bcast(&prive, 1, MPI::LONG_LONG, MASTER);
         MPI::COMM_WORLD.Bcast(&privmod, 1, MPI::LONG_LONG, MASTER);
         MPI::COMM_WORLD.Bcast(len, line, MPI::UNSIGNED, MASTER);

         // Synchronisation
         MPI::COMM_WORLD.Barrier();

         // Amount of line to be worked by a node
         int work = line/NUM_PI;
         int sendsize = work*MAX_STR_LEN;

         // Distributing ciphertext to nodes
         if(rank == MASTER){
            MPI::COMM_WORLD.Send(inmsg_ll[0]+sendsize, sendsize, MPI::LONG_LONG, 1, 0);
            MPI::COMM_WORLD.Send(inmsg_ll[0]+2*sendsize, sendsize, MPI::LONG_LONG, 2, 0);
         }
         if(rank != MASTER){
            MPI::COMM_WORLD.Recv(inmsg_ll[0], sendsize, MPI::LONG_LONG, MASTER, 0);
         }

         // Start point for each nodes
         int startline = rank*work;

         // Ciphertext decryption loop
         for(int i=0; i<work; i++){
            decrypt(inmsg_ll[i], prive, privmod, outmsg_ll[i], len[i+startline]);
            longlong2char(outmsg_ll[i], outmsg[i]);
         }

         // Collecting decrypted text back to master node
         if(rank != MASTER){
            MPI::COMM_WORLD.Send(outmsg[0], sendsize, MPI::CHAR, MASTER, 0);
         }
         if(rank == MASTER){
            MPI::COMM_WORLD.Recv(outmsg[0]+sendsize, sendsize, MPI::CHAR, 1, 0);
            MPI::COMM_WORLD.Recv(outmsg[0]+2*sendsize, sendsize, MPI::CHAR, 2, 0);
         }

         // Synchronisation before printing
         MPI::COMM_WORLD.Barrier();

         if(rank == MASTER){
            std::ofstream decrypted("decrypted.txt");
            for(int i=0; i<line; i++)
               decrypted << outmsg[i] << std::endl;
            decrypted.close();
         }

         // Final synchronisation and finalizing
         MPI::COMM_WORLD.Barrier();
         MPI::Finalize();

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
