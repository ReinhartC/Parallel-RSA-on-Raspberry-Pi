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

  char coba[6][10], tes[6][10];
  int size=20;
  if (world_rank == 0) {
    strcpy(coba[0],"Node");
    strcpy(coba[1],"0");
    strcpy(coba[2],"Node");
    strcpy(coba[3],"1");
    strcpy(coba[4],"Node");
    strcpy(coba[5],"2");
    MPI_Send(coba[0]+size,size,MPI_BYTE,1,0,MPI_COMM_WORLD);
    MPI_Send(coba[0]+2*size,size,MPI_BYTE,2,0,MPI_COMM_WORLD);
  }
  if (world_rank != 0){
    MPI_Recv(coba[0], size, MPI_UNSIGNED, 0, 0, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
    if(world_rank==1){
      strcpy(tes[0],coba[0]);
      strcpy(tes[1],coba[1]);
    }
    else if(world_rank==2){
      strcpy(tes[0],coba[0]);
      strcpy(tes[1],coba[1]);
    }
    MPI_Send(tes[0],size,MPI_BYTE,0,0,MPI_COMM_WORLD);
  }

  if (world_rank == 0) {
    strcpy(tes[0],"Ber");
    strcpy(tes[1],"hasil");
    MPI_Recv(tes[0]+size,size,MPI_BYTE,1,0,MPI_COMM_WORLD,MPI_STATUS_IGNORE);
    MPI_Recv(tes[0]+2*size,size,MPI_BYTE,2,0,MPI_COMM_WORLD,MPI_STATUS_IGNORE);
    for(int i=0; i<6; i++)
      std::cout<<tes[i]<<std::endl;
  }



  MPI_Finalize();
}
