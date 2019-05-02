#include <iostream>
#include <string.h>

int main(){
        int d[3][5]={0}, f[3][3][10];

        for(int i=0;i<5;i++) d[0][i]=i+1;
        memcpy(&f[0],&d,sizeof(d));
        
		for(int i=0;i<5;i++) d[0][i]=i+6;
		memcpy(&f[1],&d,sizeof(d));
		
        for(int i=0;i<5;i++) d[0][i]=i+11;
        memcpy(&f[2],&d,sizeof(d));
         
        for (int i=0; i<3; i++){
            for (int j=0;; j++){
                    std::cout << f[i][0][j] << " ";
                    if(f[i][0][j]==0) break;
            }
                std::cout << std::endl;
        }

        return 0;
}
