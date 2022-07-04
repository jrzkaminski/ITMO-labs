#include "mpi.h"
#include <iostream>


using namespace std;
int main(int argc, char **argv)
{
	int rank, size, new_rank;
	MPI_Init(&argc, &argv);
	MPI_Comm comm_revs;
	MPI_Comm_size(MPI_COMM_WORLD, &size);
	MPI_Comm_rank(MPI_COMM_WORLD, &rank);
	MPI_Comm_split(MPI_COMM_WORLD, rank % 2, size - rank, &comm_revs);
	MPI_Comm_rank(comm_revs, &new_rank);
	printf("Process rank: %d, New rank: %d\n", rank, new_rank);
	MPI_Comm_free(&comm_revs);
	MPI_Finalize();
}