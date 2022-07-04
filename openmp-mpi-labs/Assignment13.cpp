#include <cstdlib>
#include <time.h>
#include "mpi.h"

int random_matrix(double** m , long int n) {
	srand(time(NULL));
	for (int i = 0; i < n; i++)
		for (int j = 0; j < n; j++)
			m[i][j] = rand() / RAND_MAX;
	return 0;
}


int zero_matrix(double** m , long int n) {
	srand(time(NULL));
	for (int i = 0; i < n; i++)
		for (int j = 0; j < n; j++)
			m[i][j] = 0.0;
	return 0;
}


int mem_free(double** m , long int n) {
	for (int i = 0; i < n; i++)
		delete[] m[i];
	delete[] m;
	return 0;
}


double** mem_alloc(long int n) {
	double** m = new double*[n];
	for (int i = 0; i < n; i++)
		m[i] = new double[n];
	return m;
}


int main(int argc, char **argv)
{
	MPI_Init(&argc, &argv);
	int size, rank;
	MPI_Comm_size(MPI_COMM_WORLD, &size);
	MPI_Comm_rank(MPI_COMM_WORLD, &rank);
	int N = 500;
	double **A, **B, **C;

	A = mem_alloc(N);
	B = mem_alloc(N);
	C = mem_alloc(N);

	random_matrix(A, N);
	random_matrix(B, N);
	zero_matrix(C, N);

	MPI_Barrier(MPI_COMM_WORLD); //barrier process synchronization
	// start timing for each process
	// matrix multiplication
	// end of timing
	// output the execution time of matrix multiplication at each process
	double t_start = MPI_Wtime();
	for (int i = 0; i < N; i++)
		for (int j = 0; j < N; j++)
			for (int k = 0; k < N; k++)
				C[i][j] += A[i][k] * B[k][j];
	double t_stop = MPI_Wtime();
	double exectime = t_stop - t_start;
	printf("Process rank: %d execution time of matrix multipication: %f\n", rank, exectime);
	MPI_Finalize();
}