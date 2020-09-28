#include <iostream>
#include <fstream>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/mman.h>
#include <sys/time.h>
#include <time.h>
#include <sys/wait.h>

using namespace std; 

/* sudo gcc -othread -pthread MatMulThreads.c 
   sudo ./thread
*/

int** A;
int** B;
int** C;
int** D;
int n; 
int num_of_threads = 2;
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

void print_result(int **A, int n){
  int i,j;
  for(i = 0; i < n; i++){
      for(j = 0; j < n; j++){ 
         cout << A[i][j] << " ";
       }
      cout << endl;
   }
  cout << endl; 
}


int main(int argc, char* argv[]){
  n = (int)atoi(argv[1]); 
  pthread_t tid[num_of_threads];
  int i, j;
  long rank;

  A = new int* [n], B = new int* [n], C = new int* [n], D = new int* [n];
  for(j=0; j<n; j++){
    A[j] = new int [n], B[j] = new int [n], C[j] = new int [n],  D[j] = new int [n];
  }

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
  cout << n <<","; 
  cout << milisecs <<endl; 

  //print_result(A, n);
  //print_result(B, n);
  //print_result(C, n);
  //Liberación de memoria
  delete[] A, B, C, D; 

  // Fin de proceso padre
  pthread_exit(NULL);
  return 0;
}


