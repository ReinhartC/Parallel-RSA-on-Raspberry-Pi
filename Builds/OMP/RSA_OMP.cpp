#include <stdlib.h>
#include <math.h>
#include <limits.h>
#include <iostream>
#include <fstream>
#include <string.h>
#include <time.h>
#include <omp.h>

// 0 for Encryption (Requires Input file and Public key file)
// 1 for Decryption (Requires Input file and Private key file)
#define COMMAND 0
#define ENCRYPT_INPUT_FILE "input.txt"
#define DECRYPT_INPUT_FILE "encrypted.txt"
#define MAX_STR_LEN 10000

int encrypt(long long int* inmsg, long long int, long long int, long long int* outmsg, size_t len);
int decrypt(long long int* inmsg, long long int, long long int, long long int* outmsg, size_t len);
int char2longlong(char* in, long long int* out);
int longlong2char(long long int* in, char* out);

int main(void) {

   long long int pube, pubmod, prive, privmod;
   short int command=COMMAND;
   size_t len=0;

   char inmsg[MAX_STR_LEN];
   long long int inmsg_ll[MAX_STR_LEN];

   char outmsg[MAX_STR_LEN];
   long long int outmsg_ll[MAX_STR_LEN];

   char decrmsg[MAX_STR_LEN];
   long long int decrmsg_ll[MAX_STR_LEN];
   
   std::cout << "RSA Parallel Encryption with OpenMP" << std::endl << std::endl;
      
   switch(command){
      case 0: //Encrypt
      {
         //Public key load
      	std::ifstream pubkey("pubkey.txt");
         pubkey >> pube >> pubmod;
         pubkey.close();
         
         // Plaintext load, encryption, and ciphertext writing to output file
         std::ifstream plaintext(ENCRYPT_INPUT_FILE);
         std::ofstream encrypted("encrypted.txt");         
         
         // Plaintext encryption loop
		   while(!plaintext.eof()){
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
                     
         std::cout << "'" << ENCRYPT_INPUT_FILE << "'" << " encryption successful." << std::endl;
         break;
      }

      case 1: //Decrypt
      {  
	  	   // Private key load
	  	   std::ifstream privkey("privkey.txt");
         privkey >> prive >> privmod;
         privkey.close();
		 
		   // Ciphertext load, decryption, and writing results to output file
         std::ifstream ciphertext(DECRYPT_INPUT_FILE);
         std::ofstream decrypted("decrypted.txt");
         
         // Ciphertext decryption loop
         while(!ciphertext.eof()){
            while(ciphertext >> inmsg_ll[len]) {
               if(inmsg_ll[len]==0) break;
            	len++;
			   }

            decrypt(inmsg_ll, prive, privmod, decrmsg_ll, len);

         	longlong2char(decrmsg_ll, decrmsg);
         	decrypted << decrmsg << std::endl;
         	len=0;
         }

         ciphertext.close(); 
         decrypted.close();
         
         std::cout << "'" << DECRYPT_INPUT_FILE << "'" << " decryption successful." << std::endl;
         break;
      }
   }

}

// Encryption function
int encrypt(long long int* in, long long int exp, long long int mod, long long int* out, size_t len){
   #pragma omp parallel for default(shared)
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
   #pragma omp parallel for default(shared)
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
