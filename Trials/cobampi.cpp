#include <mpi.h>
#include <iostream>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int main(int argc, char** argv) {
  // Initialize the MPI environment
  MPI_Init(NULL, NULL);
  // Find out rank, size
  int world_rank;
  MPI_Comm_rank(MPI_COMM_WORLD, &world_rank);
  int world_size;
  MPI_Comm_size(MPI_COMM_WORLD, &world_size);

  // We are assuming at least 2 processes for this task
  if (world_size < 2) {
    fprintf(stderr, "World size must be greater than 1 for %s\n", argv[0]);
    MPI_Abort(MPI_COMM_WORLD, 1);
  }

  int snd[5][5]={0},number[5][5][5]={0};
  if (world_rank != 0) {
    if(world_rank==1){
    	for(int i=0;i<5;i++) snd[0][i]=i+6;
    	for(int i=0;i<5;i++) snd[1][i]=i+21;
	}
    else if(world_rank==2){
		for(int i=0;i<5;i++) snd[0][i]=i+11;
		for(int i=0;i<5;i++) snd[1][i]=i+26;
	}
    MPI_Send(&snd,5,MPI_INT,0,0,MPI_COMM_WORLD);
  }

  if (world_rank == 0) {
    for(int i=0;i<5;i++) snd[0][i]=i+1;
    for(int i=0;i<5;i++) snd[1][i]=i+16;
    memcpy(&number[0],&snd,sizeof(snd));

    MPI_Recv(&snd,5,MPI_INT,1,0,MPI_COMM_WORLD,MPI_STATUS_IGNORE);
    memcpy(&number[1],&snd,sizeof(snd));

    MPI_Recv(&snd,5,MPI_INT,2,0,MPI_COMM_WORLD,MPI_STATUS_IGNORE);
    memcpy(&number[2],&snd,sizeof(snd));

	for(int j=0;j<2;j++)
	    for (int i=0; i<3; i++){
	        for (auto x : number[i][j]){
	            std::cout << x << " ";
	            if(x==0) break;
	        }
	        std::cout << std::endl;
	    }
  }
  MPI_Finalize();
}
