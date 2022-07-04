#include <stdio.h>
#include "mpi.h"

int main(int argc, char** argv) {
    MPI_Init(&argc, &argv);
    
    int rank, size;

    MPI_Comm_size(MPI_COMM_WORLD, &size);
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    
    int v_size = 1000000;
    int numbers_1[v_size];
    int numbers[v_size / size];
    int totalsum[size];


    // generate vector of 1 million '1s'
    if (rank == 0) {
      for (int i = 0; i < v_size; i++)
        numbers[i] = 1;
    }

    // reduce method
    double t_start_1 = MPI_Wtime();
    MPI_Scatter(numbers, v_size / size, MPI_INT, &numbers_1, v_size / size, MPI_INT, 0, MPI_COMM_WORLD);
    long int processsum= 0;

    for (int i = 0; i < v_size / size; i++) {
      processsum= processsum+ numbers_1 [i];
    }
    long int sum = 0;
    MPI_Reduce(&processsum, &sum, 1, MPI_INT, MPI_SUM, 0, MPI_COMM_WORLD);
    if (rank == 0) {
      printf("Total sum of array: %ld\n", sum);
      double t_stop_1 = MPI_Wtime();
      double exectime_1 = t_stop_1 - t_start_1;
      printf("Execution time of reduce method: %f\n", exectime_1);
    }

    // cascade method
    double t_start = MPI_Wtime();

    MPI_Scatter(numbers, v_size / size, MPI_INT,
                &numbers_1, v_size / size, MPI_INT, 0, MPI_COMM_WORLD);
    
    processsum= 0; 

    for (int i = 0; i < v_size / size; i++) {
      processsum= processsum+ numbers_1[i];
    }
    
    MPI_Gather(&processsum, 1, MPI_INT, 
              &totalsum, 1, MPI_INT, 0, MPI_COMM_WORLD);
    
    sum = 0;

    if (rank == 0) {
      sum = 0;
      for (int i = 0; i < size; i++) {
        sum += totalsum[i];
      }
      printf("Total sum of array: %ld\n", sum);
      double t_stop = MPI_Wtime();
      double exectime = t_stop - t_start;
      printf("Execution time of cascade method: %f\n", exectime);
    }

    MPI_Finalize();
    return 0;
}