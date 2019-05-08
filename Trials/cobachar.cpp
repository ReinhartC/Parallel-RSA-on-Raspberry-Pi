#include <iostream>
#include <string.h>
#define MAX 1000


int main(){
        int d[3][5]={0}, e[3][5]={0}, g[3][5]={0}, f[3][3][10];

        for(int i=0;i<2;i++) d[0][i]=i+1;
        memcpy(&f[0],&d,sizeof(d));
        
		for(int i=2;i<4;i++) e[0][i]=i+1;
		memcpy(&f[1],&e,sizeof(e));
		
        for(int i=4;i<6;i++) g[0][i]=i+1;
        memcpy(&f[2],&g,sizeof(g));
         
        for(int j=0;j<3;j++){
			for (int i=j*2; i<2*(j+1); i++) std::cout << f[j][0][i] << " ";
		}
			
		int x=100;
		int y = x * MAX;
		std::cout << std::endl << y << std::endl;
		std::cout << sizeof(d) << " " << sizeof(f);
        return 0;
}
