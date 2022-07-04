#include <stdio.h>
#include <stdlib.h>
#include "mpi.h"


void main(int argc, char* argv[]) {
  MPI_Init(&argc, &argv);
  int rank, n, i;
  double t_start, t_stop, time;
  MPI_Status status;
  MPI_Comm_size(MPI_COMM_WORLD, &n);
  MPI_Comm_rank(MPI_COMM_WORLD, &rank);
  int L;
  int N = 10;
  int tag_1 = 10;
  int tag_2 = 20;

  for (L = 1; L <= 1e6; L *= 10) {
    int* A = (int*)malloc(L * sizeof(int));
    if (rank == 0)
      t_start = MPI_Wtime();

    for (int i = 1; i <= N; i++) {
      if (rank == 0) {
        MPI_Send(A, L, MPI_INT, 1, tag_1, MPI_COMM_WORLD);
        MPI_Recv(A, L, MPI_INT, 1, tag_2, MPI_COMM_WORLD, &status);
      } else if (rank == 1) {
        MPI_Recv(A, L, MPI_INT, 0, tag_1, MPI_COMM_WORLD, &status);
        MPI_Send(A, L, MPI_INT, 0, tag_2, MPI_COMM_WORLD);
      }
    }
    if (rank == 0) {
      t_stop = MPI_Wtime();
      time = t_stop - t_start;
      long int num_B = sizeof(int) * L;
      long int B_in_GB = 1 << 20;
      double num_GB = (double)num_B / (double)B_in_GB;
      double bandwidth = num_GB * 2.0 * (double)N / time;
      printf("Bandwidth of the message of length %d is %f MB/s\n", L, sizeof(int) * bandwidth);
    }
    free(A);
  }
  if (rank == 0)
    t_start = MPI_Wtime();

  for (int i = 1; i <= N; i++) {
    if (rank == 0) {
      MPI_Send(&i, 0, MPI_INT, 1, tag_1, MPI_COMM_WORLD);
      MPI_Recv(&i, 0, MPI_INT, 1, tag_2, MPI_COMM_WORLD, &status);
    } else if (rank == 1) {
      MPI_Recv(&i, 0, MPI_INT, 0, tag_1, MPI_COMM_WORLD, &status);
      MPI_Send(&i, 0, MPI_INT, 0, tag_2, MPI_COMM_WORLD);
    }
  }
  if (rank == 0) {
    t_stop = MPI_Wtime();
    time = t_stop - t_start;
    double latency = time / (2 * N);
    printf("Latency = %.1f ns\n", latency * 1000000000);
  }

  MPI_Finalize();
}