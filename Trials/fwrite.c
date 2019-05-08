#include <stdio.h>

int main(void) {
  FILE *key;
  int exp=123,mod=15709;
  
  key = fopen("pubkey", "w");

  putw(exp, key); 
  putw(mod, key);
    
  fclose(key);
  
  return 0;
}
