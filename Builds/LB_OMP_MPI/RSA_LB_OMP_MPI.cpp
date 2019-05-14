#include <stdlib.h>
#include <math.h>
#include <limits.h>
#include <iostream>
#include <fstream>
#include <string.h>
#include <omp.h>
#include <mpi.h>

#define MAX_STR_LEN 200
#define MAX_LINE 1002

#define CHUNK 1
#define NUM_PI 3
#define NUM_THREADS 4
#define MASTER 0

int encrypt(long long int* inmsg, long long int, long long int, long long int* outmsg, size_t len);
int decrypt(long long int* inmsg, long long int, long long int, long long int* outmsg, size_t len);
int loadBalance(double* in, int* out, int line);
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

   // Line amount and adjustment
   int line = atoi(mpinput[4]);

   // Processing Variables
   char inmsg[MAX_LINE][MAX_STR_LEN];
   long long int inmsg_ll[MAX_LINE][MAX_STR_LEN];
   char outmsg[MAX_LINE][MAX_STR_LEN];
   long long int outmsg_ll[MAX_LINE][MAX_STR_LEN];

   // Load Balancing Variables
   double lasttime[NUM_PI];
   double elapsedtime[NUM_PI];
   int load[NUM_PI];
   int sendsize[NUM_PI];

   char node_name[MPI_MAX_PROCESSOR_NAME];
   int name_len;
   MPI_Get_processor_name(node_name, &name_len);

   if (rank == MASTER)
      std::cout << "Parallel RSA with OpenMP and MPI running on node:";

   // Synchronizing before start
   MPI_Barrier(MPI_COMM_WORLD);

   // Show running nodes
   std::cout << " " << node_name << " ";

   switch(command){
      case 0: //Encrypt
      {
         if(rank == MASTER){
            // Public key load
            std::ifstream pubkey(input_key_path);
            pubkey >> pube >> pubmod;
            pubkey.close();

            // Performance summary load
            std::ifstream time("performance");
            for(int i=0; i<NUM_PI; i++)
               time >> lasttime[i];
            time.close();

            // Calculate load distribution
            loadBalance(lasttime, load, line);

            // Sendsizes for every nodes
            for(int i=0; i<NUM_PI; i++)
               sendsize[i] = load[i]*MAX_STR_LEN;

            // Plaintext load
            std::ifstream plaintext(input_file_path);
            for(int i=0; i<line; i++){
               plaintext.getline(inmsg[i],MAX_STR_LEN);
               len[i] = strlen(inmsg[i]);
            }
            plaintext.close();
         }

         // Broadcasting key to nodes
         MPI_Bcast(&pube, 1, MPI_LONG_LONG, MASTER, MPI_COMM_WORLD);
         MPI_Bcast(&pubmod, 1, MPI_LONG_LONG, MASTER, MPI_COMM_WORLD);
         MPI_Bcast(len, line, MPI_UNSIGNED, MASTER, MPI_COMM_WORLD);
         MPI_Bcast(load, NUM_PI, MPI_INT, MASTER, MPI_COMM_WORLD);
         MPI_Bcast(sendsize, NUM_PI, MPI_INT, MASTER, MPI_COMM_WORLD);

         // Synchronisation after broadcasting
         MPI_Barrier(MPI_COMM_WORLD);

         // Distributing plaintext to nodes
         if(rank == MASTER){
            MPI_Send(inmsg[0]+(sendsize[0]), sendsize[1], MPI_BYTE, 1, 0, MPI_COMM_WORLD);
            MPI_Send(inmsg[0]+(sendsize[0]+sendsize[1]), sendsize[2], MPI_BYTE, 2, 0, MPI_COMM_WORLD);
         }
         if(rank != MASTER){
            MPI_Recv(inmsg[0], sendsize[rank], MPI_BYTE, MASTER, 0, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
         }

         // Start point for each nodes
         int startline=0;
         for(int i=0; i<rank; i++)
            startline += load[i];

         // Plaintext encryption loop
         double starttime = MPI_Wtime();
         for(int i=0; i<load[rank]; i++){
            char2longlong(inmsg[i], inmsg_ll[i]);
            encrypt(inmsg_ll[i], pube, pubmod, outmsg_ll[i], len[i+startline]);
         }
         double endtime = MPI_Wtime();

         // Count elapsed time
         elapsedtime[rank] = endtime-starttime;

         // Collecting encrypted text back to master node
         if(rank != MASTER){
            MPI_Send(outmsg_ll[0], sendsize[rank], MPI_LONG_LONG, MASTER, 0, MPI_COMM_WORLD);
            MPI_Send(&elapsedtime[rank], 1, MPI_DOUBLE, MASTER, 0, MPI_COMM_WORLD);
         }
         if(rank == MASTER){
            MPI_Recv(outmsg_ll[0]+(sendsize[0]), sendsize[1], MPI_LONG_LONG, 1, 0, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
            MPI_Recv(&elapsedtime[1], 1, MPI_DOUBLE, 1, 0, MPI_COMM_WORLD, MPI_STATUS_IGNORE);

            MPI_Recv(outmsg_ll[0]+(sendsize[0]+sendsize[1]), sendsize[2], MPI_LONG_LONG, 2, 0, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
            MPI_Recv(&elapsedtime[2], 1, MPI_DOUBLE, 2, 0, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
         }

         // Synchronisation before printing
         MPI_Barrier(MPI_COMM_WORLD);

         if(rank == MASTER){
            // Printing results on master
            std::ofstream encrypted("encrypted.txt");
            for(int i=0; i<line; i++){
               if(len[i]>0){
                  for(int j=0; j<len[i]; j++)
                     encrypted << outmsg_ll[i][j] << " ";
                  encrypted << 0 << std::endl;
               }
            }
            encrypted.close();

            // Load balancing debug
            for(int i=0; i<NUM_PI; i++){
               std::cout << std::endl;
               std::cout << "Node " << i << std::endl;
               std::cout << "Load given: " << load[i] << std::endl;
               std::cout << "Elapsed time: " << elapsedtime[i] << std::endl;
            }

            // Printing nodes' performance
            std::ofstream performance("performance");
            for(int i=0; i<NUM_PI; i++)
               performance << elapsedtime[i] << " ";
            performance.close();
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

            // Performance summary load
            std::ifstream time("performance");
            for(int i=0; i<NUM_PI; i++)
               time >> lasttime[i];
            time.close();

            // Calculate load distribution
            loadBalance(lasttime, load, line);
            // Sendsizes for every nodes
            for(int i=0; i<NUM_PI; i++)
               sendsize[i] = load[i]*MAX_STR_LEN;

            // Ciphertext load
            std::ifstream ciphertext(input_file_path);
            for(int i=0; i<line; i++){
               len[i]=0;
               while(ciphertext >> inmsg_ll[i][len[i]]) {
                  if(inmsg_ll[i][len[i]]==0) break;
                  len[i]++;
               }
            }
            ciphertext.close();
         }

         // Broadcasting key to nodes
         MPI_Bcast(&prive, 1, MPI_LONG_LONG, MASTER, MPI_COMM_WORLD);
         MPI_Bcast(&privmod, 1, MPI_LONG_LONG, MASTER, MPI_COMM_WORLD);
         MPI_Bcast(len, line, MPI_UNSIGNED, MASTER, MPI_COMM_WORLD);
         MPI_Bcast(load, NUM_PI, MPI_INT, MASTER, MPI_COMM_WORLD);
         MPI_Bcast(sendsize, NUM_PI, MPI_INT, MASTER, MPI_COMM_WORLD);

         // Synchronisation
         MPI_Barrier(MPI_COMM_WORLD);

         // Distributing ciphertext to nodes
         if(rank == MASTER){
            MPI_Send(inmsg_ll[0]+(sendsize[0]), sendsize[1], MPI_LONG_LONG, 1, 0, MPI_COMM_WORLD);
            MPI_Send(inmsg_ll[0]+(sendsize[0]+sendsize[1]), sendsize[2], MPI_LONG_LONG, 2, 0, MPI_COMM_WORLD);
         }
         if(rank != MASTER){
            MPI_Recv(inmsg_ll[0], sendsize[rank], MPI_LONG_LONG, MASTER, 0, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
         }

         // Start point for each nodes
         int startline=0;
         for(int i=0; i<rank; i++)
            startline += load[i];

         // Ciphertext decryption loop
         double starttime = MPI_Wtime();
         for(int i=0; i<load[rank]; i++){
            decrypt(inmsg_ll[i], prive, privmod, outmsg_ll[i], len[i+startline]);
            longlong2char(outmsg_ll[i], outmsg[i]);
         }
         double endtime = MPI_Wtime();

         // Count elapsed time
         elapsedtime[rank] = endtime-starttime;

         // Collecting decrypted text back to master node
         if(rank != MASTER){
            MPI_Send(outmsg[0], sendsize[rank], MPI_BYTE, MASTER, 0, MPI_COMM_WORLD);
            MPI_Send(&elapsedtime[rank], 1, MPI_DOUBLE, MASTER, 0, MPI_COMM_WORLD);
         }
         if(rank == MASTER){
            MPI_Recv(outmsg[0]+(sendsize[0]), sendsize[1], MPI_BYTE, 1, 0, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
            MPI_Recv(&elapsedtime[1], 1, MPI_DOUBLE, 1, 0, MPI_COMM_WORLD, MPI_STATUS_IGNORE);

            MPI_Recv(outmsg[0]+(sendsize[0]+sendsize[1]), sendsize[2], MPI_BYTE, 2, 0, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
            MPI_Recv(&elapsedtime[2], 1, MPI_DOUBLE, 2, 0, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
         }

         // Synchronisation before printing
         MPI_Barrier(MPI_COMM_WORLD);

         // Printing results on master
         if(rank == MASTER){
            std::ofstream decrypted("decrypted.txt");
            for(int i=0; i<line; i++)
               if(len[i]>0)
                  decrypted << outmsg[i] << std::endl;
            decrypted.close();


            // Load balancing debug
            for(int i=0; i<NUM_PI; i++){
               std::cout << std::endl;
               std::cout << "Node " << i << std::endl;
               std::cout << "Load given: " << load[i] << std::endl;
               std::cout << "Elapsed time: " << elapsedtime[i] << std::endl;
            }

            // Printing nodes' performance
            std::ofstream performance("performance");
            for(int i=0; i<NUM_PI; i++)
               performance << elapsedtime[i] << " ";
            performance.close();
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

// Calculate load distribution
int loadBalance(double* in, int* out, int line){
   double work[NUM_PI];
   double sum=0;
   int total=0;

   for(int i=0; i<NUM_PI;i++){
      work[i] = 1/in[i];
      sum += work[i];
   }  
   
   for(int i=0; i<NUM_PI;i++){
      out[i] = (work[i]/sum)*line;
      total += out[i];
   }

   int node=0;
   while(total!=line){
      if(total<line) {
         out[node]++; total++;
      }
      else if(total>line){
         out[node]--; total--;
      }

      if(node<NUM_PI) node++;
      else node=0;
   }

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
