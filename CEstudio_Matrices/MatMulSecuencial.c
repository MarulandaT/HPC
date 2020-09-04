#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <sys/time.h>
#include <time.h>

/* gcc -osecuencial MatMulSecuencial.c
   ./secuencial
*/

int row = 20, col = 20;
double** A;
double** B;
double** C;

//Funciones seriales 
void read_mat(int col, int row); 
void matmul(int col, int row); 
void print_result(); 

int main(int argc, char* argv[]){
  
  int i, j;     
  long rank;

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
  read_mat(col,row);

  struct timeval start;
  struct timeval end;
  double milisecs;
  long seconds, useconds;
  
  gettimeofday(&start, 0);

  //Realizar la multiplicacion
  matmul(col, row); 
  
  gettimeofday(&end, 0);

  //Impresión de resultado
  print_result();
  seconds  = end.tv_sec  - start.tv_sec;
  useconds = end.tv_usec - start.tv_usec;
  milisecs = ((seconds) * 1000 + useconds/1000.0);
  printf("Tiempo de ejecucion:\t");
  printf("%.16g milisegundos\n", milisecs );

  //Liberación de memoria
  free(A);
  free(B);
  free(C);

  // Fin de proceso padre
  pthread_exit(NULL);
  return 0;
}

//-------------------------------------------
// Función que va ejecutarse en cada thread
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
void read_mat(int col, int row){
  int i,j;
  FILE *file;
  file=fopen("matrices.txt", "r");
  
  for(i = 0; i < row; i++){
      for(j = 0; j < col; j++){
       if (!fscanf(file, "%lf", &A[i][j])) 
           break;
      }
   }

  for(i = 0; i < row; i++){
      for(j = 0; j < col; j++){
         if (!fscanf(file, "%lf", &B[i][j])) 
             break; 
       }

   }

  fclose(file);
}

void print_result(){
  int i,j;
  FILE *file; 
  file = fopen("resultadosecuencial.txt", "w"); 
  
  for(i = 0; i < row; i++){
      for(j = 0; j < col; j++){ 
         fprintf(file, "%.0lf ", C[i][j]);
       }
      fprintf(file, "\n");
   }

  fclose (file);
}