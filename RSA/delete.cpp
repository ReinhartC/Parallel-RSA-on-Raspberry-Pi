/* C++ Program - Delete Files */
		
#include<iostream>
int main()
{
	int status;
	char fname[20]="privkey";
	status=remove(fname);
	if(status==0)
	{
		std::cout<<"file "<<fname<<" deleted successfully..!!\n";
	}
	else
	{
		std::cout<<"Unable to delete file "<<fname<<"\n";		
	}
}
