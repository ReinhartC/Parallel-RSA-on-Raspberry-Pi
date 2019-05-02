#include <iostream>
#include <string.h>

int main(){
	char a[3][10], b[3][10];
	int c[3][3]={0}, d[3][3]={0}, e[3][3][10]={0};
//	std::cin >> a[0];
//	std::cin >> a[1];
//	std::cin >> a[2];
	char teks[10]="";
	c[0][0]=1;
	c[0][1]=2;
	c[0][2]=3;
	d[0][0]=4;
	d[0][1]=5;
	d[0][2]=6;
	memcpy(&e[1],&c,sizeof(c));
	memcpy(&e[2],&d,sizeof(d));
//	std::cout << b[0];
//	std::cout << b[1];
//	std::cout << b[2];
	//int len = *(&e[0][0] + 1) - e[0][0];
	//for(int i=0; i<len; i++) std::cout << e[1][0][i] << std::endl;
	for (int i=0; i<2; i++)
		for (auto x : e[1][0]){
    		std::cout << x << std::endl;
    	
//	std::cout << e[2][0][0] << std::endl;
//	std::cout << strlen(teks);
	
	return 0;
}
