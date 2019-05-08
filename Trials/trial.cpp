#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>
#include <unistd.h>
#include <omp.h>
#include <mpi.h>
#include <time.h>
#include <string.h>

#define chunk 1
#define NUM_PI 3
#define NUM_THREADS 4
#define MASTER 0

void encrypt(long int *text, long int exp, long int mod, int size);
void decrypt(long int *text, long int exp, long int mod, int size);

int size;
int rank;

int main(int argc, char** argv) {
	
	// Setup OpemMPI
	MPI_Init(&argc, &argv);
	MPI_Comm_size(MPI_COMM_WORLD, &size);
	MPI_Comm_rank(MPI_COMM_WORLD, &rank);
	
	// Determine encryption or decryption
	int mode = atoi(argv[1]);

	//get input key
	char* input_key_path = argv[2];
	char* input_file_path = argv[3];
	
	// Text size
	FILE *key, *ifp, *ofp;
	int size32;
	int text_size;
	int size_per_proc;
	int i;
	long int exp;
	long int mod;
	long int* text; 
	long int* processed_text;
	
	if(rank == MASTER){
		// Get key
		key = fopen(input_key_path, "r");
		
		exp = getw(key);
		mod = getw(key);

		// Open files
		int size_bytes;
		int original_size;
		
		if (mode == 0){
			ifp = fopen(input_file_path, "r");
			ofp = fopen("encrypted.txt", "w");
			
		}else if (mode == 1) {
			ifp = fopen(input_file_path, "r");
			ofp = fopen("decrypted.txt", "w");
			
		}
		//CHECK IF INPUT FILE PATH IS VALID
		if (ifp == NULL) { 
			fprintf(stderr, "Can't open file in %d\n", rank);
		}
		if (ofp == NULL) {
			fprintf(stderr, "Can't open file out %d\n", rank);
		}
		
		//GET SIZE OF INPUT FILE
		fseek(ifp, 0L, SEEK_END);	
		size_bytes = ftell(ifp);
		original_size = size_bytes; 
		
		rewind(ifp);
		
		//read in input file into text
		fread(text, sizeof(int32_t), original_size/4, ifp);
		text_size = size_bytes/4;
		size_per_proc = text_size/NUM_PI;
	}
	
	//Broadcast Message size and key to all nodes
	MPI_Bcast(exp, 1, MPI_LONG_LONG, MASTER, MPI_COMM_WORLD);
	MPI_Bcast(mod, 1, MPI_LONG_LONG, MASTER, MPI_COMM_WORLD);
	MPI_Bcast(&size32, 1, MPI_INT, MASTER, MPI_COMM_WORLD);
	MPI_Bcast(&text_size, 1, MPI_INT, MASTER, MPI_COMM_WORLD);
	MPI_Bcast(&size_per_proc, 1, MPI_INT, MASTER, MPI_COMM_WORLD);

	//Allocate memory for text_sub
	long int* text_sub = malloc(sizeof(long int));
	
	//MPI Barrier for Synchronisation
	MPI_Barrier(MPI_COMM_WORLD);
	
	//SENDING DATA OUT ON MASTER
	if(rank == MASTER){ 
		MPI_Send( (text+size_per_proc), size_per_proc, MPI_LONG_LONG, 1, 0 , MPI_COMM_WORLD );
		MPI_Send( (text+2*size_per_proc), size_per_proc, MPI_LONG_LONG, 2, 0, MPI_COMM_WORLD );
		memcpy(text_sub, text, size_per_proc*4);
	}
	//RECEIVING DATA ON SLAVES	
	if(rank == 1 || rank == 2){
		MPI_Recv( text_sub, size_per_proc, MPI_LONG_LONG, 0, 0, MPI_COMM_WORLD, MPI_STATUS_IGNORE);	
		
	}

	//DETERMINE WHETHER TO DECRYPT OR ENCRYPT BASED ON ARGUMENT GIVEN
	if (mode == 0)
		encrypt(text_sub, exp, mod, size_per_proc);
	else if (mode == 1)
		decrypt(text_sub, exp, mod, size_per_proc);
		
	//SEND DATA BACK TO MASTER PI
	if(rank!=MASTER){
		MPI_Send(text_sub, size_per_proc, MPI_LONG_LONG, 0, 0, MPI_COMM_WORLD);
	}
	
	//PUT DATA BACK TOGETHER ON MASTER PI
	if(rank == MASTER){
		memcpy(processed_text, text_sub, size_per_proc*4);
		MPI_Recv( (processed_text + size_per_proc), size_per_proc, MPI_LONG_LONG, 1, 0, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
		MPI_Recv( (processed_text + 2*size_per_proc), size_per_proc, MPI_LONG_LONG, 2, 0, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
	}	
	

	//MPI Barrier for Synchronisation
	MPI_Barrier(MPI_COMM_WORLD);

	// Stop timer and verify
	if (rank == MASTER){
		printf("Writing to output\n");

		fwrite(processed_text, sizeof(long int), size32, ofp);
		fclose(ofp);	
		
	}
	
	MPI_Barrier(MPI_COMM_WORLD);
	MPI_Finalize();
	return 0;

}

void encrypt(long int *text, long int exp, long int mod, int size){
	int i;
	#pragma omp parallel for default(shared) private(i) schedule(dynamic, chunk) num_threads(4)
	for (i = 0; i < size; i += 2){
		long int c = text[i];
		int z;
		for (z=1;z<exp;z++){
        	c *= in[i];
        	c %= mod;
		}
		text[i]=c;
	}
	text[size]='\0';
}

void decrypt(long int *text, long int exp, long int mod, int size){
	int i;
	#pragma omp parallel for default(shared) private(i) schedule(dynamic, chunk) num_threads(4)
	for (i = 0; i < size; i += 2){
		long int c = text[i];
		int z;
		for (z=1;z<exp;z++){
        	c *= in[i];
        	c %= mod;
		}
		text[i]=c;
	}
	text[size]='\0';
}
