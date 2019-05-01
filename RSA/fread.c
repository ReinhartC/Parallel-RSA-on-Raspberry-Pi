#include <stdio.h>

int main(void) {
  FILE *key;
  int exp, mod;
   
  key = fopen("pubkey", "r");
  
  exp = getw(key);
  mod = getw(key);
  
  printf("%d\n%d",exp,mod);
  fclose(key);
  
  return 0;
}
