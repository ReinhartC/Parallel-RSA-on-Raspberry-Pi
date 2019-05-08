#include <stdlib.h>
#include <math.h>
#include <limits.h>
#include <iostream>
#include <fstream>
#include <time.h>

long long int prime(long long int);
long long int gcd(long long int p, long long int q);
int publickey(long long int p, long long int q, long long int* exp, long long int* mod);
int privatekey(long long int p, long long int q, long long int pubexp, long long int* exp, long long int* mod);

int main(){
   long long int p, q;
   long long int pube, pubmod, prive, privmod;

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

   std::cout << "Public key and Private key has been created"<< std::endl;
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
