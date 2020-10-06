#include <iostream>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/mman.h>
#include <sys/time.h>
#include <time.h>
#include <sys/wait.h>
#include <math.h>
#include <pthread.h>
#include <thread>
#include <vector>
#define NUM_OF_THREADS 2
//gcc -oneedl needle_thread.c -lm -pthread

using namespace std; 

double* p;
double* x;
double* y;

void lanzar(double &n, long id){
	
	long i; 
	long num_per_thr = n/NUM_OF_THREADS;
	long start_index = id * num_per_thr; 
	long final_index = start_index + num_per_thr;
	
	for(i = start_index; i < final_index; i++){
        if (x[i]*x[i] + y[i]*y[i] < 1.0){
            p[id]++;                                    
        }
	}
}

void gendata(double n, double factor) {
	for(long i = 0; i < n; i++){
	x[i] = (double)rand()*factor;       // random number (0 - 1)
	y[i] = (double)rand()*factor;       // random number (0 - 1)
	}
}

int main(int argc, char* argv[]){
	//int n = (int)atoi(argv[1]);

    //double x; // x coordinate of the dart
	//double y; // y coordinate of the dart
    double factor; // limit of rand function
    double n; // amount of trials
    //double p; // positive trials
    double pi; // output
	double aux;
    //double i; // loop counter

	srand(time(NULL));

	n = (double)atoi(argv[1]);
	p = 0;
	factor = 1.0 / RAND_MAX;

	p = new double [(long)NUM_OF_THREADS];
	x = new double [(long)n];
	y = new double [(long)n];

	struct timeval start;
	struct timeval end;
	double milisecs;
	long seconds, useconds;

    vector<thread> threads; //creacion de n hilos

	gendata(n, factor);

	gettimeofday(&start, 0);
	for (long i=0; i<NUM_OF_THREADS; i++){
		threads.push_back(thread(
			lanzar, ref(n), i
		));
	}
    for (thread &t : threads)
        t.join();
	gettimeofday(&end, 0);

	for(long i = 0; i < NUM_OF_THREADS; i++) {
		aux+= p[i];
	}

	seconds  = end.tv_sec  - start.tv_sec;
	useconds = end.tv_usec - start.tv_usec;
	milisecs = ((seconds) * 1000 + useconds/1000.0);
	cout << n << "\t" << aux <<"\t"; 
	
	pi = 4*aux/n;
	
	cout << milisecs << "\t" << pi << endl;
}
