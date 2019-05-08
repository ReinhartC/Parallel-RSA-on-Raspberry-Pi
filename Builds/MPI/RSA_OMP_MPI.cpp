#include <stdlib.h>
#include <math.h>
#include <limits.h>
#include <iostream>
#include <fstream>
#include <string.h>
#include <time.h>
#include <mpi.h>

#define MAX_STR_LEN 10000
#define MAX_LINE 1000

#define NUM_PI 3
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
   size_t len=0;

   // Process command input
   short int command=atoi(mpinput[1]);

   // Input files
   char* input_key_path = mpinput[2];
   char* input_file_path = mpinput[3];

   // Line amount
   int line = atoi(mpinput[4]);

   // Processing variables
   char inmsg[MAX_STR_LEN];
   long long int inmsg_ll[MAX_STR_LEN];
   char outmsg[MAX_STR_LEN];
   long long int outmsg_ll[MAX_STR_LEN];
   char decrmsg[MAX_STR_LEN];
   long long int decrmsg_ll[MAX_STR_LEN];

   // Result storing variables
   long long int encsend[MAX_LINE][MAX_STR_LEN];
   char decsend[MAX_LINE][MAX_STR_LEN];

   // Printing variables
   long long int encout[NUM_PI][MAX_LINE][MAX_STR_LEN];
   char decout[NUM_PI][MAX_LINE][MAX_STR_LEN];

   char node_name[MPI_MAX_PROCESSOR_NAME];
   int name_len;
   MPI_Get_processor_name(node_name, &name_len);

   if (rank == MASTER)
      std::cout << "Parallel RSA with MPI"<< std::endl << std::endl;
   
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

         // Plaintext encryption loop
         std::ifstream plaintext(input_file_path);
         for(int i=startline; i<endline; i++){
            plaintext.getline(inmsg,MAX_STR_LEN);
            len = strlen(inmsg);
            char2longlong(inmsg, inmsg_ll);

            encrypt(inmsg_ll, pube, pubmod, outmsg_ll, len);

            for(int j=0; j<len; j++)
               encsend[i][j] = outmsg_ll[j];
            encsend[i][len] = 0;
         }
         plaintext.close();

         // Size of result array to send and receive
         int sendsize = work*MAX_STR_LEN;

         // Sending worked results back to master
         if(rank != MASTER)
            MPI_Send(&encsend, sendsize, MPI_LONG_LONG, MASTER, 0, MPI_COMM_WORLD);         

         if(rank == MASTER){
            // Result array from master_node
            memcpy(&encout[0],&encsend,sizeof(encsend));
            // Result array from slave_node_1
            MPI_Recv(&encsend,sendsize,MPI_LONG_LONG,1,0,MPI_COMM_WORLD,MPI_STATUS_IGNORE);
            memcpy(&encout[1],&encsend,sizeof(encsend));
            // Result array form slave_node_2
            MPI_Recv(&encsend,sendsize,MPI_LONG_LONG,2,0,MPI_COMM_WORLD,MPI_STATUS_IGNORE);
            memcpy(&encout[2],&encsend,sizeof(encsend));

            // Printing results to output file
            std::ofstream encrypted("encrypted.txt");
            for(int i=0; i<NUM_PI; i++){
               int startprint = i*work;
               int endprint = (i+1)*work;

               for(int j=startprint; j<endprint; j++){
                  for(auto x : encout[i][j]){
                     encrypted << x << " ";
                     if(x==0) break;
                  }
                  encrypted << std::endl;
               }
            }
            encrypted.close();
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

         // Amount of line to be worked by a node
         int work = line/NUM_PI;

         // Work start and end point for each nodes
         int startline = rank*work;
         int endline = (rank+1)*work;

         // Ciphertext decryption loop
         std::ifstream ciphertext(input_file_path); 
         for(int i=startline; i<endline; i++){
            while(ciphertext >> inmsg_ll[len]) {
               if(inmsg_ll[len]==0) break;
               len++;
            }
            
            decrypt(inmsg_ll, prive, privmod, decrmsg_ll, len);

            longlong2char(decrmsg_ll, decsend[i]);
            len=0;
         }
         ciphertext.close();

         // Size of result array to send and receive
         int sendsize = work*MAX_STR_LEN;

         // Sending worked results back to master
         if(rank != MASTER)
            MPI_Send(&decsend, sendsize, MPI_BYTE, MASTER, 0, MPI_COMM_WORLD);

         if(rank == MASTER){
            // Result array from master_node
            memcpy(&decout[0],&decsend,sizeof(decsend));
            // Result array from slave_node_1
            MPI_Recv(&decsend,sendsize,MPI_BYTE,1,0,MPI_COMM_WORLD,MPI_STATUS_IGNORE);
            memcpy(&decout[1],&decsend,sizeof(decsend));
            // Result array from slave_node_2
            MPI_Recv(&decsend,sendsize,MPI_BYTE,2,0,MPI_COMM_WORLD,MPI_STATUS_IGNORE);
            memcpy(&decout[2],&decsend,sizeof(decsend));

            // Printing results to output file
            std::ofstream decrypted("decrypted.txt");
            for(int i=0; i<NUM_PI; i++){
               int startprint = i*work;
               int endprint = (i+1)*work;

               for(int j=startprint; j<endprint; j++)
                  decrypted << decout[i][j] << std::endl;
            }
            decrypted.close();      
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
