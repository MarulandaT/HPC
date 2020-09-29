#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <sys/time.h>
#include <time.h>
/* sudo gcc -othread -pthread MatMulThreads.c 
   sudo ./thread
*/

int** A;
int** B;
int** C;
int** D;
int n; 
int num_of_threads;

void *matmul(void* rank);

void create_mat(int **A, int n){
  int i,j;
  for(i = 0; i < n; i++){
      for(j = 0; j < n; j++){
       A[i][j] = rand() % 9; 
      }
   }
}

void transponer(int **D, int **B, int n) {
int i, j ; 
for(i = 0; i < n; i++){
  for(j = 0; j < n; j++){
    D[j][i] = B[i][j]; 
  }
}

}


void print_result(int **C, int n){
  int i,j;
  for(i = 0; i < n; i++){
      for(j = 0; j < n; j++){ 
        printf("%d ", C[i][j]);
         //cout << A[i][j] << " ";
       }
      //cout << endl;
      printf("\n");
   }
  //cout << endl; 
  printf("\n");
}


int main(int argc, char* argv[]){
  num_of_threads = 2;
   
  n = (int)atoi(argv[1]); 
  pthread_t tid[num_of_threads];
  int i, j;
  long rank;

  A = malloc(n*sizeof(double*));
      for(i=0;i<n;i++)
        A[i]=malloc(n*sizeof(double));

  B = malloc(n*sizeof(double*));
      for(i=0;i<n;i++)
        B[i]=malloc(n*sizeof(double));

  C =  malloc(n*sizeof(double*));
      for(i=0;i<n;i++)
        C[i]=malloc(n*sizeof(double));
  
  D =  malloc(n*sizeof(double*));
      for(i=0;i<n;i++)
        D[i]=malloc(n*sizeof(double));

  //Lectura de matrices
  create_mat(A, n); 
  create_mat(B, n);
  transponer(D, B, n);

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
  seconds  = end.tv_sec  - start.tv_sec;
  useconds = end.tv_usec - start.tv_usec;
  milisecs = ((seconds) * 1000 + useconds/1000.0);
  printf("%d,", n);
  printf("%.16g\n", milisecs );

 // print_result(A, n);
 // print_result(B, n);
 // print_result(C, n);
  //Liberación de memoria
  free(A); 
  free(B); 
  free(C); 
  free(D); 


  // Fin de proceso padre
  pthread_exit(NULL);
  return 0;
}

// Función que va ejecutarse en cada thread
void *matmul(void* id_arg){
  int i,j,k;
  long  id = (long ) id_arg;
  int rows_per_thr = n/num_of_threads;
  int start_index = id*rows_per_thr;
  int final_index;
  if (id+1==num_of_threads){
    final_index = n; 
  }
  else {
    final_index = (id+1)*rows_per_thr;
  }

  for(i=start_index;i<final_index;i++){
   for(j=0;j<n;j++){
    for(k=0;k<n;k++){
      C[i][j] += A[i][k]*D[j][k]; 
    }
   }
  }
}

