#include <stdlib.h>
#include <math.h>
#include <limits.h>
#include <iostream>
#include <fstream>
#include <string.h>
#include <time.h>

#define MAX_STR_LEN 200
#define MAX_LINE 1002

int encrypt(long long int* inmsg, long long int, long long int, long long int* outmsg, size_t len);
int decrypt(long long int* inmsg, long long int, long long int, long long int* outmsg, size_t len);
int char2longlong(char* in, long long int* out);
int longlong2char(long long int* in, char* out);

int main(int init, char** input) {

   long long int pube, pubmod, prive, privmod;
   size_t len[MAX_LINE];

   // Process command input
   short int command=atoi(input[1]);

   // Input files
   char* input_key_path = input[2];
   char* input_file_path = input[3];

   // Line amount
   int line = atoi(input[4]);

   // Processing Variables
   char inmsg[MAX_LINE][MAX_STR_LEN];
   long long int inmsg_ll[MAX_LINE][MAX_STR_LEN];
   char outmsg[MAX_LINE][MAX_STR_LEN];
   long long int outmsg_ll[MAX_LINE][MAX_STR_LEN];

   std::cout << "RSA Parallel Encryption with OpenMP" << std::endl << std::endl;

   switch(command){
      case 0: //Encrypt
      {
         //Public key load
         std::ifstream pubkey(input_key_path);
         pubkey >> pube >> pubmod;
         pubkey.close();

         // Plaintext load
         std::ifstream plaintext(input_file_path);
         for(int i=0; i<line; i++){
            plaintext.getline(inmsg[i],MAX_STR_LEN);
            len[i] = strlen(inmsg[i]);
         }
         plaintext.close();

         // Plaintext encryption loop
         for(int i=0; i<line; i++){
            char2longlong(inmsg[i], inmsg_ll[i]);
            encrypt(inmsg_ll[i], pube, pubmod, outmsg_ll[i], len[i]);
         }

         // Printing results
         std::ofstream encrypted("encrypted.txt");
         for(int i=0; i<line; i++){
            if(len[i]>0){
               for(int j=0; j<len[i]; j++)
                  encrypted << outmsg_ll[i][j] << " ";
               encrypted << 0 << std::endl;
            }
         }
         encrypted.close();

         std::cout << "'" << input_file_path << "'" << " encryption successful." << std::endl;
         break;
      }

      case 1: //Decrypt
      {
         // Private key load
         std::ifstream privkey(input_key_path);
         privkey >> prive >> privmod;
         privkey.close();

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

         // Ciphertext decryption loop
         for(int i=0; i<line; i++){
            decrypt(inmsg_ll[i], prive, privmod, outmsg_ll[i], len[i]);
            longlong2char(outmsg_ll[i], outmsg[i]);
         }

         // Printing results
         std::ofstream decrypted("decrypted.txt");
         for(int i=0; i<line; i++)
            if(len[i]>0)
               decrypted << outmsg[i] << std::endl;
         decrypted.close();


         std::cout << "'" << input_file_path << "'" << " decryption successful." << std::endl;
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
