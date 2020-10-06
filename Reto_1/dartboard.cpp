#include <iostream>
#include <fstream>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/mman.h>
#include <sys/time.h>
#include <time.h>
#include <sys/wait.h>
#include <math.h>

using namespace std;

double x; // x coordinate of the dart
double y; // y coordinate of the dart
double factor; // limit of rand function
double n; // amount of trials
double p; // positive trials
double pi; // output
double i; // loop counter

void lanzar();

int main(int argc, char* argv[]){
	//int n = (int)atoi(argv[1]);

	srand(time(NULL));

	n = (double)atoi(argv[1]);
	p = 0;
	factor = 1.0 / RAND_MAX;

	struct timeval start;
	struct timeval end;
	double milisecs;
	long seconds, useconds;

	gettimeofday(&start, 0);
	for (i=0; i<n; i++){
		lanzar();
	}
	gettimeofday(&end, 0);

	seconds  = end.tv_sec  - start.tv_sec;
	useconds = end.tv_usec - start.tv_usec;
	milisecs = ((seconds) * 1000 + useconds/1000.0);
	cout << n << "\t" << p <<"\t"; 
	pi = 4*p/n;
	cout << milisecs << "\t" << pi << endl;
}

void lanzar(){
	x = (double)rand()*factor;       // random number (0 - 1)
	y = (double)rand()*factor;       // random number (0 - 1)
	if (x*x + y*y < 1.0){
		p++;                                    
	}
}