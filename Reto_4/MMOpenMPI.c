#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <mpi.h>
#include <omp.h>

//mpicc -fopenmp MMOpenMPI.c -o exec
//export OMP_NUM_THREADS=4
//mpirun -np 4 -hosts head,wn1,wn2,wn3 ./exec 32
//mpirun -np 4 -machinefile mfile ./exec 32

void matMul(int n, int numranks, int rank, double* mat2, double* scatterMat, double* gatherMat);
void writeTime(double tiempo, int tam, int wnodos);
void printMat(double* mat, int n);

int main(int argc, char *argv[]){
    int n = atoi(argv[1]);
    int numranks, rank, len;
    char hostname[MPI_MAX_PROCESSOR_NAME];

    double startTime;
    double endTime;
    double tiempo;

    double *mat1 = (double *)malloc(n*n*sizeof(double));
    double *mat2 = (double *)malloc(n*n*sizeof(double));

    srand(time(NULL));
    for(int i = 0; i < n; i++){
        for(int j = 0; j < n; j++){
            mat1[i*n+j] = rand() % 10;
            mat2[i*n+j] = rand() % 10;
        }
    }

    MPI_Init(&argc, &argv);
    MPI_Comm_size(MPI_COMM_WORLD, &numranks);
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    MPI_Get_processor_name(hostname, &len);

    double *scatterMat = (double *)malloc((n*n/numranks)*sizeof(double));
    double *gatherMat = (double *)malloc((n*n/numranks)*sizeof(double));
    double *result = (double *)malloc(n*n*sizeof(double));

    MPI_Bcast(mat2, n*n, MPI_DOUBLE, 0, MPI_COMM_WORLD);

    startTime = MPI_Wtime();

    MPI_Scatter(&mat1[(n*n/numranks)*rank], n*n/numranks, MPI_DOUBLE, scatterMat, n*n/numranks, MPI_DOUBLE, 0, MPI_COMM_WORLD);
    MPI_Barrier(MPI_COMM_WORLD);
    
    matMul(n, numranks, rank, mat2, scatterMat, gatherMat);

    MPI_Gather(gatherMat, n*n/numranks, MPI_DOUBLE, &result[(n*n/numranks)*rank], n*n/numranks, MPI_DOUBLE, 0, MPI_COMM_WORLD);
    MPI_Barrier(MPI_COMM_WORLD);

    endTime = MPI_Wtime();

    if(rank == 0){	
        tiempo = endTime - startTime;
        writeTime(tiempo, n, numranks);
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

    MPI_Finalize();

    return 0;
}

void matMul(int n, int numranks, int rank, double* mat2, double* scatterMat, double* gatherMat){
    int fil, col, k, sum;
    int my_id, num_threads;
    int start_index, end_index, rows_per;
    #pragma omp parallel firstprivate(mat2, scatterMat, gatherMat), private(fil, col, k, sum, my_id, num_threads, start_index, end_index, rows_per)
    {
        my_id = omp_get_thread_num();
        num_threads = omp_get_num_threads();
        rows_per = (n / numranks) / num_threads; 
        start_index = my_id * rows_per; 
        end_index = start_index + rows_per;
        sum = 0;
        printf("Soy el hilo %i del procesador %i\n", my_id, rank);
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

void writeTime(double tiempo, int tam, int wnodos){
    FILE *f = fopen("timesP2PMPI.txt","a+");
    fprintf(f,"%i;%i;%.6lf\n", wnodos, tam, tiempo);
    fclose(f);
}