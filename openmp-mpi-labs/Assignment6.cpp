#include <iostream>
#include <mpi.h>
using namespace std;
int main(int argc, char **argv)
{
	int rank, size, ibuf;
	MPI_Status status;
	float rbuf;
	MPI_Init(&argc, &argv);
	MPI_Comm_size(MPI_COMM_WORLD, &size); // MPI communicator size
	MPI_Comm_rank(MPI_COMM_WORLD, &rank); // ranks for each process
	ibuf = rank;
	rbuf = 1.0 * rank; //rbuf integer -> float
  // processes 1, 2 send their ranks
	if (rank == 1) MPI_Send(&ibuf, 1, MPI_INT, 0, 1, MPI_COMM_WORLD);
	if (rank == 2) MPI_Send(&rbuf, 1, MPI_FLOAT, 0, 2, MPI_COMM_WORLD);
	if (rank == 0) {
		for (int i = 1; i < 3; i++) { // loop to recieve both messages properly 
			MPI_Probe(MPI_ANY_SOURCE, i, MPI_COMM_WORLD, &status); // recieve message attempt
			if (status.MPI_TAG == 1) { 
				MPI_Recv(&ibuf, 1, MPI_INT, 1, 1, MPI_COMM_WORLD, &status); // recieving integer from process 2
				cout << "Process 0 recv " << ibuf << " from process 1\n";
			}
			else if (status.MPI_TAG == 2) { 
				MPI_Recv(&rbuf, 1, MPI_FLOAT, 2, 2, MPI_COMM_WORLD, &status); //receiving float from process 2
				cout << "Process 0 recv " << rbuf << " from process 2\n";// print it
			}
		}
	}
	MPI_Finalize();
}