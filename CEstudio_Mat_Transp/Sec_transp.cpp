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
/* g++ -osecuencial Sec_transp.cpp
   ./secuencial
*/

//Funciones seriales 
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

void matmul(int **A, int **B, int **C, int n){
  int i,j,k;

  for(i = 0; i < n; i++) {
    for(j = 0; j < n; j++) {
      C[i][j] = 0; 
      for(k = 0; k < n; k++) {
        C[i][j] += A[i][k]*B[j][k]; 
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
  //int row = (int)atoi(argv[1]), col = (int)atoi(argv[1]);
  int i, j, n=(int)atoi(argv[1]);     

  int **A, **B, **C, **D; 
  A = new int* [n], B = new int* [n], C = new int* [n], D = new int* [n];
  for(j=0; j<n; j++){
    A[j] = new int [n], B[j] = new int [n], C[j] = new int [n],  D[j] = new int [n];
  }

  /*A[0][0] = 1; 
  A[1][0] = 0; 
  A[0][1] = 7;  
  A[1][1] = 7; 
  B[0][0] = 5;
  B[1][0] = 1;
  B[0][1] = 7;
  B[1][1] = 3;*/

  //Lectura de matrices
  create_mat(A, n);
  create_mat(B, n);
  transponer(D,B,n); 
  //print_result(D, n);

  struct timeval start;
  struct timeval end;
  double milisecs;
  long seconds, useconds;
  
  gettimeofday(&start, 0);

  //Realizar la multiplicacion
  matmul(A, D, C, n); 
  
  gettimeofday(&end, 0);

  //Impresión de resultado
  //print_result(col, row);
  seconds  = end.tv_sec  - start.tv_sec;
  useconds = end.tv_usec - start.tv_usec;
  milisecs = ((seconds) * 1000 + useconds/1000.0);
  cout << "Tamano matriz: \t" << n <<"\t"; 
  cout << "Tiempo de ejecucion: \t" << milisecs <<endl; 
  
  //print_result(A, n); 
  //print_result(B, n);
  //print_result(C, n); 

  delete [] A, B, C, D; 

  return 0;
}



