#include <stdio.h>
#include "mpi.h"
#define n 1000

using namespace std;


void smod5(void *ibuf, void* rbuf, int *l, MPI_Datatype *type) {
    int* in = (int*)ibuf;
    int* ret = (int*)rbuf;
    for (int i = 0; i < *l; i++)
        if (in[i] >= ret[i]) ret[i] = in[i];
}


int main(int argc, char **argv)
{
	int rank, size, i;
	int a[n];
	int b[n];
    int b_tocompare[n];
	MPI_Init(&argc, &argv);
	MPI_Op op;
	MPI_Comm_size(MPI_COMM_WORLD, &size);
	MPI_Comm_rank(MPI_COMM_WORLD, &rank);
	for (i = 0; i < n; i++) a[i] = i + rank + 1;
	printf("process %d a[0] = %d\n", rank, a[0]);
	MPI_Op_create(&smod5, 1, &op);
	MPI_Reduce(a, b, n, MPI_INT, op , 0, MPI_COMM_WORLD);
	MPI_Op_free(&op);
    MPI_Reduce(a, b_tocompare, n, MPI_INT, MPI_MAX, 0, MPI_COMM_WORLD);
    if (rank ==0){
        printf("b[0] = %d\n", b[0]);
        printf("b[0] = %d\n", b_tocompare[0]);
    }
	MPI_Finalize();
}