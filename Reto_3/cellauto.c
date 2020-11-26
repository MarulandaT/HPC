#include <stdio.h>
#include <stdlib.h>
#include <time.h>

void calcular(int* street1, int* street2, int n, int p, int *mov){
	for(int i = p*n+1; i < (p+1)*n+1; i++){
		if(street1[i] == 0){
			if(street1[i-1] == 1){
				street2[i] = 1;
				*mov = *mov + 1;
			}
		}
		else{
			if(street1[i+1] == 0){
				street2[i] = 0;
			}
			else if(street1[i+1] == 1){
				street2[i] = 1;
			}
		}
	}
}

int main(int arg, char *argv[]){
	int n = 16;
	int p = 4;
	int numv = 0;
	int nummov = 0;
	float speed = 0;

	int *street1 = (int *)malloc((n+2)*sizeof(double));
	int *street2 = (int *)malloc((n+2)*sizeof(double));

	srand(time(NULL));

	for(int i = 1; i <= n; i++){
		street1[i] = rand() % 2;
		if (street1[i] == 1){
			numv = numv + 1;
		}
	}
	street1[0] = street1[n];
	street1[n+1] = street1[1];

	for(int i = 0; i < p; i++){
		calcular(street1, street2, n/p, i, &nummov);
	}
	street2[0] = street2[n];
	street2[n+1] = street2[1];

	speed = 1.0*nummov/numv;

	/*
	printf("t:\t");
	for(int i = 0; i <= n+1; i++){
		printf("%d ", street1[i]);
	}
	printf("\n");
	printf("t+1:\t");
	for(int i = 0; i <= n+1; i++){
		printf("%d ", street2[i]);
	}
	printf("\n");
	printf("espacios: %d\tprocesos: %d\tvehiculos: %d\tmovimientos: %d\t velocidad: %.2f\n", n, p, numv, nummov, speed);
	*/
}