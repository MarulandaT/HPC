
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/mman.h>
#include <sys/time.h>
#include <time.h>
#include <sys/wait.h>
#include <math.h>
#include <pthread.h>
#define NUM_OF_THREADS 2 
//gcc -oneedl needle_thread.c -lm -pthread

double PI; // limit of rand function for angle

double x; // x coordinate of needle's center 
double k; // angle between vertical position and needle
double l; // lenght of the needle
double n; // amount of trials
double p[NUM_OF_THREADS]; // positive trials per thread
double y; // sin(angle) * l
double pi; // output
double i; // loop counter



void *lanzar(void* rank);

int main(){
//int main(int argc, char* argv[]){
	//int n = (int)atoi(argv[1]);

	srand(time(NULL));
	int i; //iterador
	double aux = 0; //variable que sumará los casos de exito 

	n = 100000000; //cantidad de pruebas
	
	l = 1; 
	PI = acos(-1.0);

	pthread_t tid[NUM_OF_THREADS]; //creacion de n hilos 
	long rank; 

	struct timeval start;
	struct timeval end;
	double milisecs;
	long seconds, useconds;
  
	gettimeofday(&start, 0);

	for(rank = 0; rank<NUM_OF_THREADS; rank++)
		pthread_create(&tid[rank], NULL, lanzar, (void*) rank);

	for(rank = 0; rank<NUM_OF_THREADS; rank++)
		pthread_join(tid[rank], NULL);
	
	gettimeofday(&end, 0);

	for (i = 0; i < NUM_OF_THREADS; i++){
		aux += p[i];
	}
		

	seconds  = end.tv_sec  - start.tv_sec;
	useconds = end.tv_usec - start.tv_usec;
	milisecs = ((seconds) * 1000 + useconds/1000.0);
	printf("%g, %g\n", n, milisecs); 

	pi = (l/aux)*(n);
	printf("%g, %g\n", aux, pi);
	pthread_exit(NULL);
}

void *lanzar(void* id_arg){

	int i; 
	long id = (long) id_arg; 
	int num_per_thr = n/NUM_OF_THREADS;

	for(i = 0; i < num_per_thr; i++){
		k = (double)rand()/(RAND_MAX)*2*PI;      // random angle (0 - 2PI)
		x = (double)rand()/(RAND_MAX)/2;         // random x (0 - 0.5)
		y = (l/2) * sin (k);
		if (x<=y){
			p[id]++;              
		}
	}
}