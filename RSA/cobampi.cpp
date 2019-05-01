#include <mpi.h>
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

  int snd[5][5],number[5][5],i,j;
  if (world_rank != 0) {
    if(world_rank==1)
      for(i=0;i<5;i++) snd[1][i]=i;
    else if(world_rank==2)
      for(i=0;i<5;i++) snd[2][i]=i+5;
    MPI_Send(&snd,1,MPI_INT,0,0,MPI_COMM_WORLD);
  } 

  if (world_rank == 0) {
    MPI_Recv(&snd,1,MPI_INT,1,0,MPI_COMM_WORLD,MPI_STATUS_IGNORE);
    memcpy(&number,&snd,sizeof(snd));
    MPI_Recv(&snd,1,MPI_INT,2,0,MPI_COMM_WORLD,MPI_STATUS_IGNORE);
    memcpy(&number,&snd,sizeof(snd));

    printf("Process 1 received number %d from process sl1\n", number[1][1]);
    printf("Process 1 received number %d from process sl2\n", number[2][1]);
  }
  MPI_Finalize();
}