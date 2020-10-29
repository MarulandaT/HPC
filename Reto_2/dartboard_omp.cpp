#include <iostream>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/mman.h>
#include <sys/time.h>
#include <time.h>
#include <sys/wait.h>
#include <math.h>
#include <omp.h>
#define NUM_OF_THREADS 4
//g++ -o dartboard_omp -fopenmp dartboard_omp.cpp

using namespace std;  

void lanzar(double n, double *p, double *x, double *y){
	
	long i; 
	long num_per_thr;
	long start_index; 
	long final_index;
	
	#pragma omp parallel firstprivate(p,x,y), private (i, num_per_thr, start_index, final_index), num_threads(NUM_OF_THREADS)
	{
	num_per_thr = n / omp_get_num_threads();
	start_index = omp_get_thread_num() * num_per_thr; 
	final_index = start_index + num_per_thr;

	for(i = start_index; i < final_index; i++){
        if (x[i]*x[i] + y[i]*y[i] < 1.0){
            p[omp_get_thread_num()]++;                                    
        }
	}
	}
}

void gendata(double n, double factor, double *x, double *y) {
	for(long i = 0; i < n; i++){
	x[i] = (double)rand()*factor;       // random number (0 - 1)
	y[i] = (double)rand()*factor;       // random number (0 - 1)
	}
}

int main(int argc, char* argv[]){
	
    double factor; // limit of rand function
    double n; // amount of trials
    double pi; // output
	double aux; //Aciertos de todos los threads
	double *p; //Aciertos 
	double *x; //Posición del dardo en x
	double *y; //Posición del dardo el y 

	srand(time(NULL));

	n = (double)atoi(argv[1]);
	factor = 1.0 / RAND_MAX; //Factor para numeros aleatorios

	//Reserva de Memoria 
	p = new double [(long)NUM_OF_THREADS];
	x = new double [(long)n];
	y = new double [(long)n];

	//Variables para calcular los tiempos.
	struct timeval start;
	struct timeval end;
	double milisecs;
	long seconds, useconds;

	//Generar un conjunto de datos para los lanzamientos
	gendata(n, factor, x, y);

	gettimeofday(&start, 0);
	//Hacer los lanzamientos
	lanzar(n, p, x, y);
	gettimeofday(&end, 0);

	//Acumular la cantidad de aciertos en cada thread
	for(long i = 0; i < NUM_OF_THREADS; i++) {
		aux+= p[i];
	}

	seconds  = end.tv_sec  - start.tv_sec;
	useconds = end.tv_usec - start.tv_usec;
	milisecs = ((seconds) * 1000 + useconds/1000.0);
	cout << n << "\t" << aux <<"\t"; 
	
	pi = 4*aux/n;
	
	cout << milisecs << "\t" << pi << endl;
	return 0; 
}
