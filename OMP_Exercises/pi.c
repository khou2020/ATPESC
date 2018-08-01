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

int main(){
	int nthread = 4;
	double x, pi;
	double sum[16][PAD];

	if (argc > 1) {
		nthread = atoi(argv[1]);
	}

	omp_set_num_threads(nthread);

	step = 1.0 / (double)num_steps;

	double tdata = omp_get_wtime();

#pragma omp parallel
	{
		int tid, nt;
		double x;
		register double lsum = 0;
		int s, t;
		register int i;

		nt = omp_get_num_threads();
		tid = omp_get_thread_num();

		if (tid == 0) {
			nthread = nt;
		}

		s = num_steps / nt + 1;
		t = s * (tid + 1);
		s = t - s;

		//printf("%d, %d, %d\n", omp_get_thread_num(), s, t);
		for (i = s; i < t; i++) {
			x = ((double)i + 0.5) * step;
			lsum += 4.0 / (1.0 + x * x);
		}

		sum[tid][0] = lsum;
	}

	for (int i = 1; i < nthread; i++) {
		sum[0][0] += sum[i][0];
	}
	pi = step * sum[0][0];

	tdata = omp_get_wtime() - tdata;
	printf(" pi = %f in %f secs\n", pi, tdata);

	return 0;
}
