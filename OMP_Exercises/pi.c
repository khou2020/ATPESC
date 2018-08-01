/*

This program will numerically compute the integral of

                  4/(1+x*x) 
				  
from 0 to 1.  The value of this integral is pi -- which 
is great since it gives us an easy way to check the answer.

The is the original sequential program.  It uses the timer
from the OpenMP runtime library

History: Written by Tim Mattson, 11/99.

*/
#include <stdio.h>
#include <omp.h>

#define PAD 16

static long num_steps = 100000000;
double step;

#include <omp.h> 
#include <stdlib.h>
#include <stdio.h>

static long num_steps = 1000000000;
double step;

int main (int argc, char* argv[]) {
	int nthread = 4;
	int i;
	double x, pi, sum = 0.0;

	if (argc > 1) {
		nthread = atoi(argv[1]);
	}
	omp_set_num_threads(nthread);

	step = 1.0 / (double)num_steps; 

	double tdata = omp_get_wtime();

#pragma omp parallel
	{	
#pragma omp for private(x) reduction(+: sum) schedule(static)
		for (i = 0; i < num_steps; i++) {
			x = ((double)i + 0.5)*step;
			sum += 4.0 / (1.0 + x*x);
		}
	}
	pi = step * sum; 

	tdata = omp_get_wtime() - tdata; 
	printf(" pi = %f in %f secs\n", pi, tdata);

	return 0;
}