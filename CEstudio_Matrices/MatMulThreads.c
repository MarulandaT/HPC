#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <sys/time.h>
#include <time.h>

/* sudo gcc -othread -pthread MatMulThreads.c 
   sudo ./thread
*/

//Variables globales
int num_of_threads = 2;
int row = 20, col = 20;
double** A;
double** B;
double** C;

//Funciones seriales
void read_mat(int col, int row);
void print_result();

//Funcion paralela
void *matmul(void* rank);

int main(int argc, char* argv[]){
  
  pthread_t tid[num_of_threads];
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

  //Creación de threads
  for (rank = 0; rank < num_of_threads; rank++)
     pthread_create(&tid[rank], NULL,matmul , (void*) rank);

  //Unión de threads
  for (rank = 0; rank < num_of_threads; rank++)
      pthread_join(tid[rank], NULL);

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
void *matmul(void* id_arg){
  int i,j,k;
  long  id = (long ) id_arg;
  int rows_per_thr = col/num_of_threads;
  int start_index = id*rows_per_thr;
  int final_index;
  if (id+1==num_of_threads){
    final_index = row; 
  }
  else {
    final_index = (id+1)*rows_per_thr;
  }

  for(i=start_index;i<final_index;i++){
   for(j=0;j<col;j++){
    for(k=0;k<row;k++){
      C[i][j] += A[i][k]*B[k][j]; 
    }
   }
  }
}
//--------------------------------------------
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
  file = fopen("resultadothreads.txt", "w"); 
  for(i = 0; i < row; i++){
      for(j = 0; j < col; j++){ 
         fprintf(file,"%.0lf ",C[i][j]); 
       }
      fprintf(file, "\n");
   }

}