#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <mpi.h>
#include <omp.h>

/*
***Command for compile
mpicc -fopenmp MMOpenMPI.c -o run

***Command for specify the # of threads on each node
***Change 8 for # of threads (1,2,4,8)
export OMP_NUM_THREADS=8

***Command for run
***Change 4 for the # of working nodes (1,2,4) and 32 for matriz size (preferably exponents of 2 starting from 32)
mpirun -np 4 -machinefile mfile ./run 32
*/

void matMul(int n, int *hilos, int numranks, int rank, double* mat2, double* scatterMat, double* gatherMat);
void writeTime(int wnodos, int nhilos, int tam, double tiempo);
void printMat(double* mat, int n);

int main(int argc, char *argv[]){
    //array size (nxn)
    int n = atoi(argv[1]);
    //save the number of threads to use
    int nhilos;
    //variables for time measure
    int numranks, rank, len;

    //variables to measure time
    double startTime;
    double endTime;
    double tiempo;

    //memory space reservation
    double *mat1 = (double *)malloc(n*n*sizeof(double));
    double *mat2 = (double *)malloc(n*n*sizeof(double));

    //assign random values ​​for both arrays
    srand(time(NULL));
    for(int i = 0; i < n; i++){
        for(int j = 0; j < n; j++){
            mat1[i*n+j] = rand() % 10;
            mat2[i*n+j] = rand() % 10;
        }
    }

    //start MPI zone
    MPI_Init(&argc, &argv);
    MPI_Comm_size(MPI_COMM_WORLD, &numranks);
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);

    //memory space reservation for auxiliary arrays
    double *scatterMat = (double *)malloc((n*n/numranks)*sizeof(double));
    double *gatherMat = (double *)malloc((n*n/numranks)*sizeof(double));
    double *result = (double *)malloc(n*n*sizeof(double));

    //start timing
    startTime = MPI_Wtime();

    //send array B to all working nodes
    MPI_Bcast(mat2, n*n, MPI_DOUBLE, 0, MPI_COMM_WORLD);

    //send segment of array A that each node must process
    MPI_Scatter(&mat1[(n*n/numranks)*rank], n*n/numranks, MPI_DOUBLE, scatterMat, n*n/numranks, MPI_DOUBLE, 0, MPI_COMM_WORLD);
    MPI_Barrier(MPI_COMM_WORLD);
    
    //MPI kernel - Multiplicacion de Matrices
    matMul(n, &nhilos, numranks, rank, mat2, scatterMat, gatherMat);

    //gather the results obtained in each node
    MPI_Gather(gatherMat, n*n/numranks, MPI_DOUBLE, &result[(n*n/numranks)*rank], n*n/numranks, MPI_DOUBLE, 0, MPI_COMM_WORLD);
    MPI_Barrier(MPI_COMM_WORLD);

    //end timing
    endTime = MPI_Wtime();

    //writing time and print arrays content
    if(rank == 0){	
        tiempo = endTime - startTime;
        writeTime(numranks, nhilos, n, tiempo);
        /*
        printf("\ntiempo: %3f\n", tiempo);
        printf("A\n");
        printMat(mat1, n);
        printf("B\n");
        printMat(mat2, n);
        printf("Result\n");
        printMat(result, n);
        */
	}

    //end MPI zone
    MPI_Finalize();

    return 0;
}

void matMul(int n, int *nhilos, int numranks, int rank, double* mat2, double* scatterMat, double* gatherMat){
    //private variables of each thread
    int fil, col, k, sum;
    int my_id, num_threads;
    //indexes to divide the work
    int start_index, end_index, rows_per;
    //parallel zone OpenMP
    #pragma omp parallel firstprivate(mat2, scatterMat, gatherMat), private(fil, col, k, sum, my_id, num_threads, start_index, end_index, rows_per), shared(nhilos)
    {
        my_id = omp_get_thread_num();
        num_threads = omp_get_num_threads();
        *nhilos = num_threads;
        rows_per = (n / numranks) / num_threads; 
        start_index = my_id * rows_per; 
        end_index = start_index + rows_per;
        sum = 0;
        //printf("Soy el hilo %i del procesador %i\n", my_id, rank);
        //Matrix multiplication
        for(fil = start_index; fil < end_index; fil++){
            for(col = 0; col < n; col++){
                for(k = 0; k < n; k++){
                    sum = sum + mat2[n*k+col]*scatterMat[n*fil+k];
                }
                gatherMat[n*fil+col] = sum;
                sum = 0;
            }
        }
    }
}

//print the arrays content in console
void printMat(double* mat, int n){
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

//stores the time measure in txt file
void writeTime(int wnodos, int nhilos, int tam, double tiempo){
    FILE *f = fopen("timesOpenMPI.txt","a+");
    fprintf(f,"%i;%i;%i;%.6lf\n", wnodos, nhilos, tam, tiempo);
    fclose(f);
}