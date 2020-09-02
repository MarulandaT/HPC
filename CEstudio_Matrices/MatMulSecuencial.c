#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <time.h> 

/*gcc -osecuencial MatMulSecuencial.c
 ./secuencial
*/

int row = 1000, col = 1000;
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

  clock_t t_ini, t_fin;
  double secs;
  
  t_ini = clock();
  //Realizar la multiplicacion
  matmul(col, row); 
  t_fin = clock();

  //Impresión de resultado
  print_result();
  secs = (double)(t_fin - t_ini) / CLOCKS_PER_SEC;
  printf("%ld\n", CLOCKS_PER_SEC);
  printf("%.16g milisegundos\n", secs );

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
  file=fopen("matext1000x1000.txt", "r");
  
 printf(" Primera matriz: \n");
  for(i = 0; i < row; i++){
      for(j = 0; j < col; j++){
       if (!fscanf(file, "%lf", &A[i][j])) 
           break;
      printf("%.0lf ",A[i][j]);
      }
      printf("\n");
   }

  printf("\n Segunda matriz \n");

  for(i = 0; i < row; i++){
      for(j = 0; j < col; j++){
         if (!fscanf(file, "%lf", &B[i][j])) 
             break; 
         printf("%.0lf ",B[i][j]); 
       }
      printf("\n");
   }
  fclose(file);
}

void print_result(){
  int i,j;
  printf("\n Matriz resultado: \n");
  for(i = 0; i < row; i++){
      for(j = 0; j < col; j++){ 
         printf("%.0lf ",C[i][j]); 
       }
      printf("\n");
   }

}
