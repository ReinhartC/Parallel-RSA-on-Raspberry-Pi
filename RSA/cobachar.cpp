#include <iostream>
#include <string.h>

int main(){
	char a[3][10], b[3][10];
	int c[3][3], d[3][3], e[3][3][3];
//	std::cin >> a[0];
//	std::cin >> a[1];
//	std::cin >> a[2];

	c[1][1]=1;
	c[1][2]=2;
	c[1][3]=3;
	d[1][1]=4;
	d[1][2]=5;
	d[1][3]=6;
	memcpy(&e[1],&c,sizeof(c));
	memcpy(&e[2],&d,sizeof(d));
//	std::cout << b[0];
//	std::cout << b[1];
//	std::cout << b[2];
	std::cout << e[1][1][1] << std::endl;
	std::cout << e[2][1][1] << std::endl;
	
	return 0;
}
