#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/mman.h>
#include <sys/time.h>
#include <time.h>
#include <sys/wait.h>
#include <chrono>
#include "./csv_generator.h"
//g++ -o fork MatMulFork.cpp 

void matmul (int ** A , int ** B ,int ** C, int number,long i);
void solve (int ** A , int ** B , int  ** C, int number);
void create(int ** A , int number);
void print_result(int ** m, int number);

int main (int argc, char *argv[]){
    int number = (int)atoi(argv[1]);
    //int number = 4; 

    //CREACION DE LAS MATRICES A Y B 
    int  ** A;
    int  ** B; 
    //Multiplicacion de las matrices A x B = C 
    int  ** C;

    A = (int **) mmap(NULL,sizeof(int *)*number,PROT_READ | PROT_WRITE,MAP_SHARED | MAP_ANONYMOUS,1,0);
    B = (int **) mmap(NULL,sizeof(int *)*number,PROT_READ | PROT_WRITE,MAP_SHARED | MAP_ANONYMOUS,1,0);
    C = (int **) mmap(NULL,sizeof(int *)*number,PROT_READ | PROT_WRITE,MAP_SHARED | MAP_ANONYMOUS,1,0);

    for (int i = 0; i< number;i++){
        A[i] = (int*)mmap(NULL,sizeof(int *)*number,PROT_READ | PROT_WRITE,MAP_SHARED | MAP_ANONYMOUS,1,0);
        B[i] = (int*)mmap(NULL,sizeof(int *)*number,PROT_READ | PROT_WRITE,MAP_SHARED | MAP_ANONYMOUS,1,0);
        C[i] = (int*)mmap(NULL,sizeof(int *)*number,PROT_READ | PROT_WRITE,MAP_SHARED | MAP_ANONYMOUS,1,0);
    }

    create(A,number);
    create(B,number);

    struct timeval start;
    struct timeval end;
    double milisecs;
    long seconds, useconds;

    gettimeofday(&start, 0);
    solve(A,B,C,number);
    gettimeofday(&end, 0);

    seconds  = end.tv_sec  - start.tv_sec;
    useconds = end.tv_usec - start.tv_usec;
    milisecs = ((seconds) * 1000 + useconds/1000.0);

    //printf("Tiempo de ejecucion:\t");
    //printf("%.16g milisegundos\n", milisecs );
    csv_time(milisecs,number);
    //print_result(A,number);
    //print_result(B,number);
    //print_result(C,number);

    return 1;
}

void matmul (int ** A , int ** B ,int ** C, int number,long i){
    for (long j = 0; j < number; j ++){
        int sum = 0;
        for (long x = 0; x < number; x ++ )
            sum = sum + A[j][x] * B[x][i];
        C[j][i] = sum;

    }
}

void solve (int ** A , int ** B , int  ** C, int number){
    int status;
    pid_t p_id;

    for (long i = 0; i<number; i++) {
        p_id = fork();
        if (p_id == 0){
            matmul(A,B,C,number,i);
            //printf("mi identificador de proceso es: %d\n", getpid());
            exit(1);
        }else continue;
        }

    for (long i = 0; i < number;i++) wait(&status);

}

void create(int ** A , int number){
    for (int i = 0;i<number;i++){
        for (int j = 0 ; j<number;j++){
             A[i][j] = rand() % 9;
        }
       
    }
}

void print_result(int ** m, int number){

    for (int i = 0;i<number;i++){
        for (int j = 0 ; j<number;j++){
            printf(" %d ", m[i][j] );
        }
        printf("\n");
    }
    printf("\n");
}

