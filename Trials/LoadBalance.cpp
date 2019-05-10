#include <iostream>

using namespace std;


int main(){
	float time[3];
	float work[3];
	int load[3];
	float line=3291;
	
	time[0]=12;
	time[1]=48;
	time[2]=240;
	
	for(int i=0; i<3;i++) 
		work[i]= 1/time[i];
	
	float sum = work[0]+work[1]+work[2];
	float remain = 1-sum;
	
	for(int i=0; i<3;i++) 
		work[i]+=remain*work[i]/sum;
	
	for(int i=0; i<3;i++) 
		load[i]=work[i]*line;
		
	int node=0;
	while(load[0]+load[1]+load[2]!=line){
		if(load[0]+load[1]+load[2]<line) 
			load[node]++;
		else if(load[0]+load[1]+load[2]>line) 
			load[node]--;
		
		if(node<3) node++;
		else node=0;
	}
	
	cout << endl << "Adjusted load balance" << endl;
	cout<< load[0] << endl;
	cout<< load[1] << endl;
	cout<< load[2] << endl; 
	cout << load[0]+load[1]+load[2]<<endl;
	cout << "Actual line: "<< line << endl;
	
	return 0;
}
