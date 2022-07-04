#include <stdio.h>
#include <chrono>
#include <iostream>
#include "mpi.h"

#define NTIMES 100

using namespace std;

int main(int argc, char* argv[]) {
  MPI_Init(&argc, &argv);
  double time_start, time_finish;
  int rank, message, worldsize, i, len;
  char *name = new char;
  MPI_Status status;
  MPI_Comm_size(MPI_COMM_WORLD, &message);
  MPI_Comm_size(MPI_COMM_WORLD, &worldsize);
  MPI_Comm_rank(MPI_COMM_WORLD, &rank);
  MPI_Get_processor_name(name, &len);
  time_start = MPI_Wtime();


  if (rank != 0) {
    MPI_Recv(&message, 1, MPI_INT, rank - 1,
             MPI_ANY_TAG, MPI_COMM_WORLD, &status);

    int dest = (rank == (worldsize - 1)) ? 0 : rank + 1;
    MPI_Send(&rank, 1, MPI_INT, dest, 0, MPI_COMM_WORLD);

  } else {
    MPI_Send(&rank, 1, MPI_INT, 1, 0, MPI_COMM_WORLD);
    MPI_Probe(worldsize - 1, MPI_ANY_TAG, MPI_COMM_WORLD, &status);
    if (status.MPI_SOURCE == worldsize - 1) {
      MPI_Recv(&worldsize - 1, 1, MPI_INT, rank - 1,
               MPI_ANY_TAG, MPI_COMM_WORLD, &status);
      cout << "Hello from process " << message << "\n";
    }
  }
  time_finish = MPI_Wtime();
  cout << "processor " << name << ", process " << rank << "time = " << \
  							(time_finish - time_start) / NTIMES << endl;
  MPI_Finalize();
}