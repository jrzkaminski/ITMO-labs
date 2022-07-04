#include <iostream>
#include "mpi.h"


int main(int argc, char **argv)
{
	MPI_Init(&argc, &argv);
	int rank, size;
	MPI_Request reqs[4];
	MPI_Status stats[4];
	MPI_Comm_size(MPI_COMM_WORLD, &size);
	MPI_Comm_rank(MPI_COMM_WORLD, &rank);
	int rbuf[2];
	int sbuf[2];
	int prev = rank - 1;
	int next = rank + 1;
	if (rank == 0) prev = size - 1;
	if (rank == size - 1) next = 0;
	// request for receive from the previous process
	MPI_Recv_init(&rbuf[0], 1, MPI_INT, prev, 5, MPI_COMM_WORLD, &reqs[0]);
	// request for receive from the next process
	MPI_Recv_init(&rbuf[1], 1, MPI_INT, next, 6, MPI_COMM_WORLD, &reqs[1]); 
	// receive for sending from the previous process
	MPI_Send_init(&sbuf[0], 1, MPI_INT, prev, 6, MPI_COMM_WORLD, &reqs[2]);
	// recieve for sending from the next process
	MPI_Send_init(&sbuf[1], 1, MPI_INT, next, 5, MPI_COMM_WORLD, &reqs[3]); 
	for (int i = 0; i < 2; i++) {
		sbuf[0] = rank;
		sbuf[1] = rank;
		MPI_Startall(4, reqs); // request start
		MPI_Waitall(4, reqs, stats); // wait for all
		// messages output
		printf("Process rank: %d r-buffer[%d] = %d\n", rank, i, rbuf[i]); 
	}
	MPI_Request_free(&reqs[0]);
	MPI_Request_free(&reqs[1]);
	MPI_Request_free(&reqs[2]);
	MPI_Request_free(&reqs[3]);

	MPI_Finalize();
	return 0;
}