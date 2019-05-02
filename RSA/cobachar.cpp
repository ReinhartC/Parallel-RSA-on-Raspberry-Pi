#include <iostream>
#include <string.h>
#define MAX 1000


int main(){
        int d[3][5]={0}, f[3][10];

        for(int i=0;i<6;i+=3) d[0][i]=i+1;
        memcpy(&f,&d,sizeof(d));
        
		for(int i=1;i<6;i+=3) d[0][i]=i+1;
		memcpy(&f,&d,sizeof(d));
		
        for(int i=2;i<6;i+=3) d[0][i]=i+1;
        memcpy(&f,&d,sizeof(d));
         
        for (int i=0; i<6; i++) std::cout << f[0][i] << " ";
		int x=100;
		int y = x * MAX;
		std::cout << std::endl << y << std::endl;
        return 0;
}
