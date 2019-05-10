#include <iostream>

using namespace std;


int main(){
	float time[3];
	float work[3];
	float sum;
	int load[3];
	float line=100;
	
	time[0]=167;
	time[1]=81;
	time[2]=483;
//
   for(int i=0; i<3;i++){
      work[i] = 1/time[i];
      sum += work[i];
   }	
	
   for(int i=0; i<3;i++) 
      load[i] = (work[i]/sum)*line;
	
   int node=0;
   int total = load[0]+load[1]+load[2];

   while(total!=line){
      if(total<line) load[node]++;
      else if(total>line) load[node]--;

      total = load[0]+load[1]+load[2];

      if(node<3) node++;
      else node=0;
   }
//	
	cout << endl << "Adjusted load balance" << endl;
	cout<< load[0] << endl;
	cout<< load[1] << endl;
	cout<< load[2] << endl; 
	cout << load[0]+load[1]+load[2] << endl;
	cout << "Actual line: "<< line << endl;
	
	return 0;
}
