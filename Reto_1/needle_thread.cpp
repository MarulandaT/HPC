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

void lanzar(double &n, double &l,  double &p, double &PI){
	
	long i; 
	double num_per_thr = n/NUM_OF_THREADS;
	//double start_index = id * num_per_thr; 
	//double final_index = start_index + num_per_thr;
	double x; 
	double y; 

	for(i = 0; i < num_per_thr; i++){
		//double k = (double)rand()/(RAND_MAX)*2*PI;      // random angle (0 - 2PI)
		 x = (double)rand()/(RAND_MAX)/2;         		  // random x (0 - 0.5)
		 y = (l/2) * sin ((double)rand()/(RAND_MAX)*2*PI);
		if (x <= y){
			p++;              
		}
	}
}

int main(int argc, char* argv[]){

	double PI; // PI Real Value
	double p; // positive trials per thread
	double pi; // output
	double l; // lenght of the needle
	double n; // amount of trials

	srand(time(NULL));
	n = (double)atoi(argv[1]);
	//n = 10000; //cantidad de pruebas
	l = 1; 

	PI = acos(-1.0); 

	vector<thread> threads; //creacion de n hilos 

	struct timeval start;
	struct timeval end;
	double milisecs;
	long seconds, useconds;

	gettimeofday(&start, 0);
	for(long i = 0; i < NUM_OF_THREADS; i++) {
		threads.push_back(thread(
			lanzar, ref(n), ref(l), ref(p), ref(PI)
		));
	}
	
	for (thread &t : threads)
        t.join();
	gettimeofday(&end, 0);

	seconds  = end.tv_sec  - start.tv_sec;
	useconds = end.tv_usec - start.tv_usec;
	milisecs = ((seconds) * 1000 + useconds/1000.0);
	cout << n << "\t" << p <<"\t"; 

	pi = (l/p)*(n);

	cout << milisecs << "\t" << pi << endl;
	pthread_exit(NULL);
	return 0;
}

