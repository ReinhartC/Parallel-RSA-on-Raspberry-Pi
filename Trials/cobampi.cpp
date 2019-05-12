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

  int angka[6][10];
  int size=1*10;
  if (world_rank == 0) {
    for(int i=0; i<10; i++) angka[1][i]=i;
    for(int i=11; i<20; i++) angka[2][i]=i;
    MPI_Send(angka[0]+size,size,MPI_INT,1,0,MPI_COMM_WORLD);
    MPI_Send(angka[0]+2*size,size,MPI_INT,2,0,MPI_COMM_WORLD);
  }
  if (world_rank != 0){
    MPI_Recv(angka[0], size, MPI_INT, 0, 0, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
    if(world_rank==1){
      for(int i=0; i<10; i++) std::cout<<angka[0][i];
    }
    else if(world_rank==2){
      for(int i=11; i<20; i++) std::cout<<angka[0][i];
    }
  }



  MPI_Finalize();
}
