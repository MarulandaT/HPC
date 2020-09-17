#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/mman.h>
#include <sys/time.h>
#include <time.h>
#include <sys/wait.h>

/* gcc -osecuencial MatMulSecuencial.c
   ./secuencial
*/

double** A;
double** B;
double** C;

//Funciones seriales 
void create_mat(int col, int row); 
void matmul(int col, int row); 
void print_result(); 

int main(int argc, char* argv[]){
  int row = (int)atoi(argv[1]), col = (int)atoi(argv[1]);
  int i, j;     

  A = malloc(row*sizeof(double*)); 
      for(i=0;i<row;i++)
        A[i]=malloc(col*sizeof(double));

  B = malloc(row*sizeof(double*)); 
      for(i=0;i<row;i++)
        B[i]=malloc(col*sizeof(double)); 
  
  C =  malloc(row*sizeof(double*)); 
      for(i=0;i<row;i++)
        C[i]=malloc(col*sizeof(double));

  //Lectura de matrices
  create_mat(col,row);

  struct timeval start;
  struct timeval end;
  double milisecs;
  long seconds, useconds;
  
  gettimeofday(&start, 0);

  //Realizar la multiplicacion
  matmul(col, row); 
  
  gettimeofday(&end, 0);

  //Impresión de resultado
  //print_result(col, row);
  seconds  = end.tv_sec  - start.tv_sec;
  useconds = end.tv_usec - start.tv_usec;
  milisecs = ((seconds) * 1000 + useconds/1000.0);
  printf("Tamano matriz: \t %d ", row);
  printf("Tiempo de ejecucion:\t");
  printf("%.16g milisegundos\n", milisecs );
  //Liberación de memoria
  free(A);
  free(B);
  free(C);

  return 0;
}

//-------------------------------------------

void matmul(int col, int row){
  int i,j,k;

  for(i=0;i<row;i++){
   for(j=0;j<col;j++){
    C[i][j]=0; 
    for(k=0;k<col;k++){
      C[i][j] += A[i][k]*B[k][j]; 
    }
   }
  }

}
//----------------------------------------
void create_mat(int col, int row){
  int i,j;
  
  for(i = 0; i < row; i++){
      for(j = 0; j < col; j++){
       A[i][j] = rand() % 9; 
      }
   }

  for(i = 0; i < row; i++){
      for(j = 0; j < col; j++){
         B[i][j] = rand() % 9; 
       }

   }
}

void print_result(int col, int row){
  int i,j;
  
  printf("Matriz A : \n");  
  for(i = 0; i < row; i++){
      for(j = 0; j < col; j++){ 
         printf("%.0lf ", A[i][j]);
       }
      printf("\n");
   }

  printf("Matriz B : \n");  
  for(i = 0; i < row; i++){
      for(j = 0; j < col; j++){ 
         printf("%.0lf ", B[i][j]);
       }
      printf("\n");
   }

  printf("Matriz C : \n");  
  for(i = 0; i < row; i++){
      for(j = 0; j < col; j++){ 
         printf("%.0lf ", C[i][j]);
       }
      printf("\n");
   }

}