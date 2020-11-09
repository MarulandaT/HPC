#include <stdio.h>
#include <stdlib.h>
#include<stdio.h>
#include<stdlib.h>
#include <time.h>
#include <omp.h>
#include <mpi.h>

void writeTime(double elapsed, int len, int npro){
		/*
			Wite the result on output.txt file
			M -> Matrix, Mrow -> Matrix rows, Mcol -> Matrix columns
		*/
		FILE *f = fopen("timescmpiMatrices.txt","a+");//write at end of file and set result, append
		//float value=;
		fprintf(f,"%i;%i;%.3lf\n", npro, len, elapsed);
		fclose(f);
}


void printMat(double* mat, int n);
int main(int argc, char *argv[]){
    //int n = atoi(argv[1]); //matrixes are n x n
    int n = 4;
	
    //Allocating Memory and Assigning Values

    //Start MPI
    //Finding Matrix Product  and Printing
    int numranks, rank, len;
    char hostname[MPI_MAX_PROCESSOR_NAME];

    double *mat1 = (double *)malloc(n*n*sizeof(double));
    double *mat2 = (double *)malloc(n*n*sizeof(double));

    double startTime;
    double endTime;
    double tiempo;

    MPI_Init(&argc, &argv);
    MPI_Comm_size(MPI_COMM_WORLD, &numranks);
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    MPI_Get_processor_name(hostname, &len);
   
    double *scatterMat = (double *)malloc((n+1)*sizeof(double));
    double *gatherMat = (double *)malloc((n+1)*sizeof(double));
    double *result = (double *)malloc(n*n*sizeof(double));

    if(rank == 0 && n % numranks != 0)
    {
        //printf("N: %d, numranks: %d\n", n, numranks);
        exit(1);
    }

    

    if(rank == 0)
    {
        for(int i = 0; i < n; i++)
        {
            for(int j = 0; j < n; j++)
            {
                mat1[i*n+j] = rand() % 11;
                mat2[i*n+j] = rand() % 11;
            }
        }
        //printf("Num tasks %d\nN: %d\n", numranks, n);
    }
    //Matrix B is copied to every processor
    MPI_Bcast(mat2, n*n, MPI_DOUBLE, 0, MPI_COMM_WORLD);
    //Print matricies
    if(rank == 0)
    {
       printf("Matrix A\n");
       printMat(mat1, n);
       printf("Matrix B\n");
      printMat(mat2, n);
    }
    
    //Matrix A is divided into blocks along the rows and distributed
    //among processors. 
    int root = 0;
    startTime = MPI_Wtime();
    
    for(int x = 0; x < n*n; x = x + numranks*n)
    {
        MPI_Scatter(&mat1[x], n, MPI_DOUBLE, scatterMat, n, MPI_DOUBLE, root, MPI_COMM_WORLD); 
        MPI_Barrier(MPI_COMM_WORLD);

	    double sum = 0.0;

    	//Multiply 
    	for(int i = 0; i < n; i++)
    	{
        	for(int j = 0; j < n; j++)
        	{
            		sum = sum + mat2[j*n+i] * scatterMat[j];
        	}
        	gatherMat[i] = sum;
        	sum = 0.0;
    	}

    	MPI_Gather(gatherMat, n, MPI_DOUBLE, &result[x], n, MPI_DOUBLE, 0, MPI_COMM_WORLD);
    	MPI_Barrier(MPI_COMM_WORLD);

    }
    
    endTime = MPI_Wtime();
    if(rank == 0)
    {	
      tiempo = endTime - startTime;
      writeTime(tiempo, n, numranks);
    	
	}
    printf("Time to complete %f\n", endTime - startTime);
    
    if(rank == 0)
    {
        printf("Result\n");
        printMat(result, n);
    }


    MPI_Finalize();

    return 0;
}


//Print singular matrix
void printMat(double* mat, int n)
{
    for(int i = 0; i < n; i++)
    {
        for(int j = 0; j < n; j++)
        {
            printf("%.0f\t", mat[i*n+j]);
        }
        printf("\n");
    }
    printf("\n");
}
