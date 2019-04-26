#include <stdlib.h>
#include <math.h>
#include <limits.h>
#include <iostream>
#include <fstream>
#include <string.h>
#include <time.h>
#include <omp.h>

// 0 for Generating Public and Private key pair files
// 1 for Encryption (Requires Input file and Public key file)
// 2 for Decryption (Requires Input file and Private key file)
#define COMMAND 2
#define ENCRYPT_INPUT_FILE "input100k.txt"
#define DECRYPT_INPUT_FILE "encrypted.txt"
#define MAX_STR_LEN 10000

long long int prime(long long int);
long long int gcd(long long int p, long long int q);
int publickey(long long int p, long long int q, long long int* exp, long long int* mod);
int privatekey(long long int p, long long int q, long long int pubexp, long long int* exp, long long int* mod);
int encrypt(long long int* inmsg, long long int, long long int, long long int* outmsg, size_t len);
int decrypt(long long int* inmsg, long long int, long long int, long long int* outmsg, size_t len);
int char2longlong(char* in, long long int* out);
int longlong2char(long long int* in, char* out);

int main(void) {

   long long int p,q, pube, pubmod, prive, privmod;
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
      case 0: //Generate Keys
      {
		 // Generate prime numbers (p and q)
         srand (time(NULL));

         p=rand() % 100+1;
         while(prime(p)!=0) p++;
         
         q=p+rand() % 1000+1;
         while(prime(q)!=0 || q==p) q++;
      	 
      	 //Generating public and private key
         publickey(p,q,&pube,&pubmod);
         privatekey(p,q,pube,&prive,&privmod);
         
         // Writing public and private key to a file
         std::ofstream pubkey("pubkey.txt");
         pubkey << pube << " " << pubmod;
         pubkey.close();
		          
         std::ofstream privkey("privkey.txt");
         privkey << prive << " " << privmod;
         privkey.close();
         
         std::cout << "Public key and Private key has been created.";
         break;
      }

      case 1: //Encrypt
      {
      	 //Public key load
      	 std::ifstream pubkey("pubkey.txt");
         while(!pubkey.eof()){
               pubkey >> pube >> pubmod;
         }
         pubkey.close();
         
         
         // Plaintext load, encryption, and ciphertext writing to output file
         std::ifstream plaintext(ENCRYPT_INPUT_FILE);
         std::ofstream encrypted("encrypted.txt");         
         // Plaintext encryption loop
		 clock_t encStart = clock();
		 while(!plaintext.eof()){
            plaintext.getline(inmsg,MAX_STR_LEN);
            len = strlen(inmsg);         			
			char2longlong(inmsg, inmsg_ll);
            encrypt(inmsg_ll, pube, pubmod, outmsg_ll, len);
            for(int i=0; i<len; i++) 
               encrypted << outmsg_ll[i] << " ";
            encrypted << 0 << std::endl;                          
         }		 	 
		 plaintext.close();                
		 encrypted.close();
                     
         std::cout << "'" << ENCRYPT_INPUT_FILE << "'" << " encryption successful." << std::endl;
         std::cout << "Elapsed time: " << (double)(clock() - encStart)/CLOCKS_PER_SEC << "s";
         break;
      }

      case 2: //Decrypt
      {  
	  	 // Private key load
	  	 std::ifstream privkey("privkey.txt");
         while(!privkey.eof()){
            privkey >> prive >> privmod;
         }
         privkey.close();
		 
		 // Ciphertext load, decryption, and writing results to output file
         std::ifstream ciphertext(DECRYPT_INPUT_FILE);
         std::ofstream decrypted("decrypted.txt");
         // Ciphertext decryption loop
         clock_t decStart = clock();
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
         std::cout << "Elapsed time: " << (double)(clock() - decStart)/CLOCKS_PER_SEC << "s";
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
