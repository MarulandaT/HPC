#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <mpi.h>

//mpicc cellautoP2P-MPI.c -o execP2P
//mpirun -np 8 -hosts wn1,wn2,wn3,wn4,wn5,wn6,wn7,wn8 ./execP2P 16 1
//mpirun -np 8 -machinefile mfile ./execP2P 16 1

//write time in txt file
void writeTime(double tiempo, int tam, int wnodos, int iterations){
	FILE *f = fopen("timesP2PMPI.txt","a+");
	fprintf(f,"%i;%i;%i;%.6lf\n", wnodos, tam, iterations, tiempo);
	fclose(f);
}

int main(int argc, char *argv[]){
	int n = atoi(argv[1]);
	int t = atoi(argv[2]);

	int numranks, rank, len;
	int tag1 = 1;
	int tag2 = 11;
	char hostname[MPI_MAX_PROCESSOR_NAME];

	//variables for timing
	double startTime;
	double endTime;
	double tiempo;

	//array containing starting street with random vehicle positions
	int *street1 = (int *)malloc((n+2)*sizeof(double));

	srand(time(NULL));

	//randomly assignation for vehicle positions
	for(int i = 1; i <= n; i++){
		street1[i] = rand() % 2;
	}
	street1[0] = street1[n];
	street1[n+1] = street1[1];

	//MPI Initialice
	MPI_Init(&argc, &argv);
	MPI_Comm_size(MPI_COMM_WORLD, &numranks);
	MPI_Comm_rank(MPI_COMM_WORLD, &rank);
	MPI_Get_processor_name(hostname, &len);

	/*
	if(rank==0){
		printf("\nt:\t");
		for(int i = 1; i < n+1; i++){
			printf("%d ", street1[i]);
		}
		printf("\n");
	}
	*/

	//array for internal processing of each node
	int *scatterStreet = (int *)malloc((n/numranks+2)*sizeof(double));
	int *gatherStreet = (int *)malloc((n/numranks+2)*sizeof(double));
	//array with final results
	int *street2 = (int *)malloc((n+2)*sizeof(double));

	//initial time
	startTime = MPI_Wtime();

	//split the array street to each node
	MPI_Scatter(&street1[rank*n/numranks+1], (n/numranks), MPI_INT, &scatterStreet[1], (n/numranks), MPI_INT, 0, MPI_COMM_WORLD);
	MPI_Barrier(MPI_COMM_WORLD);

	int prev = rank - 1;
	int next = rank + 1;
	if(prev < 0){
		prev = numranks - 1;
	}
	if(next == numranks){
		next = 0;
	}

	//initial comunication P2P between nodes to get extra positions
	if(numranks > 1){
		MPI_Send(&scatterStreet[1], 1, MPI_INT, prev, tag1*rank, MPI_COMM_WORLD);
		MPI_Recv(&scatterStreet[n/numranks+1], 1, MPI_INT, next, tag1*next, MPI_COMM_WORLD, MPI_STATUS_IGNORE);

		MPI_Send(&scatterStreet[n/numranks], 1, MPI_INT, next, tag2*rank, MPI_COMM_WORLD);
		MPI_Recv(&scatterStreet[0], 1, MPI_INT, prev, tag2*prev, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
	}
	MPI_Barrier(MPI_COMM_WORLD);

	//internal processing of each node
	for(int r = 0; r < t; r++){
		for(int i = 1; i < n/numranks+1; i++){
			if(scatterStreet[i] == 0){
				if(scatterStreet[i-1] == 1){
					gatherStreet[i] = 1;
				}
				else if(scatterStreet[i-1] == 0){
					gatherStreet[i] = 0;
				}
			}
			else if(scatterStreet[i] == 1){
				if(scatterStreet[i+1] == 0){
					gatherStreet[i] = 0;
				}
				else if(scatterStreet[i+1] == 1){
					gatherStreet[i] = 1;
				}
			}
		}
		prev = rank - 1;
		next = rank + 1;
		if(prev < 0){
			prev = numranks - 1;
		}
		if(next == numranks){
			next = 0;
		}
		//comunication P2P between nodes to get extra positions
		if(numranks > 1){
			MPI_Send(&gatherStreet[1], 1, MPI_INT, prev, tag1*rank, MPI_COMM_WORLD);
			MPI_Recv(&gatherStreet[n/numranks+1], 1, MPI_INT, next, tag1*next, MPI_COMM_WORLD, MPI_STATUS_IGNORE);

			MPI_Send(&gatherStreet[n/numranks], 1, MPI_INT, next, tag2*rank, MPI_COMM_WORLD);
			MPI_Recv(&gatherStreet[0], 1, MPI_INT, prev, tag2*prev, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
		}
		MPI_Barrier(MPI_COMM_WORLD);

		for(int i = 0; i < n/numranks+2; i++){
			scatterStreet[i] = gatherStreet[i];
		}
		MPI_Barrier(MPI_COMM_WORLD);
	}
	
	//joining the information of each node in the final array
	MPI_Gather(&gatherStreet[1], n/numranks, MPI_INT, &street2[(n/numranks)*rank+1], n/numranks, MPI_INT, 0, MPI_COMM_WORLD);
	MPI_Barrier(MPI_COMM_WORLD);

	street2[n+1] = street2[1];
	street2[0] = street2[n];

	//end time
	endTime = MPI_Wtime();
	if(rank == 0){
		tiempo = endTime - startTime;
	}

	//save times into txt file
	if(rank == 0){
		writeTime(tiempo, n, numranks, t);
		/*
		printf("\nt+1:\t");
		for(int i = 1; i < n+1; i++){
			printf("%d ", street2[i]);
		}
		printf("\nprocesos: %d\ttiempo: %f\n", numranks, tiempo);
		*/
	}

	MPI_Finalize();
}