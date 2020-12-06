#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <mpi.h>
#include <omp.h>

//mpicc -fopenmp MMOpenMPI.c -o exec
//mpirun -np 4 -hosts head,wn1,wn2,wn3 ./exec 8
//mpirun -np 4 -machinefile mfile ./exec 8

void writeTime(double tiempo, int tam, int wnodos);

void printMat(double* mat, int n);

void Matmul(int n, int numranks, double* mat2, double* scatterMat, double* gatherMat);

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
    /*
    int sum = 0;

    for(int fil = 0; fil < n/numranks; fil++){
        for(int col = 0; col < n; col++){
            for(int k = 0; k < n; k++){
                sum = sum + mat2[n*k+col]*scatterMat[n*fil+k];
            }
            gatherMat[n*fil+col] = sum;
            sum = 0;
        }
    }
    */
    Matmul(n, numranks, mat2, scatterMat, gatherMat);

    MPI_Gather(gatherMat, n*n/numranks, MPI_DOUBLE, &result[(n*n/numranks)*rank], n*n/numranks, MPI_DOUBLE, 0, MPI_COMM_WORLD);
    MPI_Barrier(MPI_COMM_WORLD);

    endTime = MPI_Wtime();

    if(rank == 0){	
        tiempo = endTime - startTime;
        printf("\ntiempo: %3f\n", tiempo);
        //writeTime(tiempo, n, numranks);
        printf("A\n");
        printMat(mat1, n);
        printf("B\n");
        printMat(mat2, n);
        printf("Result\n");
        printMat(result, n);
	}

    MPI_Finalize();

    return 0;
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

void Matmul(int n, int numranks, double* mat2, double* scatterMat, double* gatherMat){
    int sum = 0;

    for(int fil = 0; fil < n/numranks; fil++){
        for(int col = 0; col < n; col++){
            for(int k = 0; k < n; k++){
                sum = sum + mat2[n*k+col]*scatterMat[n*fil+k];
            }
            gatherMat[n*fil+col] = sum;
            sum = 0;
        }
    }
}