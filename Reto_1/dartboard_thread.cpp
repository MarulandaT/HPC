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

void lanzar(double &n, double &p, double &factor){
	
	long i; 
	double num_per_thr = n/NUM_OF_THREADS;
	//double start_index = id * num_per_thr; 
	//double final_index = start_index + num_per_thr;
	double x; 
	double y; 

	for(i = 0; i < num_per_thr; i++){
		x = (double)rand()*factor;       // random number (0 - 1)  
		y = (double)rand()*factor;       // random number (0 - 1)
        if (x*x + y*y < 1.0){
            p++;                                    
        }
	}
}

int main(int argc, char* argv[]){
	//int n = (int)atoi(argv[1]);

    double x; // x coordinate of the dart
    double y; // y coordinate of the dart
    double factor; // limit of rand function
    double n; // amount of trials
    double p; // positive trials
    double pi; // output
    //double i; // loop counter

	srand(time(NULL));

	n = (double)atoi(argv[1]);
	p = 0;
	factor = 1.0 / RAND_MAX;

	struct timeval start;
	struct timeval end;
	double milisecs;
	long seconds, useconds;

    vector<thread> threads; //creacion de n hilos

	gettimeofday(&start, 0);
	for (long i=0; i<NUM_OF_THREADS; i++){
		threads.push_back(thread(
			lanzar, ref(n), ref(p), ref(factor)
		));
	}
    for (thread &t : threads)
        t.join();
	gettimeofday(&end, 0);

	seconds  = end.tv_sec  - start.tv_sec;
	useconds = end.tv_usec - start.tv_usec;
	milisecs = ((seconds) * 1000 + useconds/1000.0);
	cout << n << "\t" << p <<"\t"; 
	pi = 4*p/n;
	cout << milisecs << "\t" << pi << endl;
}
