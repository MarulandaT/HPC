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

double PI = acos(-1.0); // limit of rand function for angle

double x; // x coordinate of needle's center 
double k; // angle between vertical position and needle
double l; // lenght of the needle
double n; // amount of trials
double p; // positive trials
double y; // sin(angle) * l
double pi; // output
double i; // loop counter

void lanzar();

int main(int argc, char* argv[]){
	n = (double)atoi(argv[1]);

	srand(time(NULL));

	//n = 1000;
	p = 0;
	l = 1;

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
	pi = (l/p)*(n);
	cout << milisecs << "\t" << pi << endl;
}

void lanzar(){
	k = (double)rand()/(RAND_MAX)*2*PI;      // random angle (0 - 2PI)
	x = (double)rand()/(RAND_MAX)/2;         // random x (0 - 0.5)
	y = (l/2) * sin (k);
	if (x<=y){
		p++;                                    
	}
}