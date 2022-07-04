#include <stdio.h>
#include <stdlib.h>
#include "mpi.h"

#define V_SIZE 1000000

//read vector
void read_vector(
    float local_v[], 
    int n_bar, 
    int p, 
    int rank, 
    int value
) {
    int i, q;
    float* temp = (float*)malloc(sizeof(float) * V_SIZE);
    MPI_Status status;
    if (rank == 0) {
        for (i = 0; i < n_bar; i++)
            local_v[i] = value;
        for (q = 1; q < p; q++) {
            for (i = 0; i < n_bar; i++)
                temp[i] = value;
            MPI_Send (temp, n_bar, MPI_FLOAT, q, 0, MPI_COMM_WORLD);

        }
    } else {
        MPI_Recv(local_v, n_bar, MPI_FLOAT, 0, 0, MPI_COMM_WORLD, &status);
    }
}


float Serial_dot(
    float x[],
    float y[],
    int n
) {
    int i;
    float sum = 0.0;
    for (i = 0; i < n; i++)
        sum = sum + x[i] * y[i];
    return sum;
}


float Parallel_dot(
    float local_x[],
    float local_y[],
    int n_bar
) {
    float local_dot;
    float dot = 0.0;
    float Serial_dot(
    float x[],
    float y[],
    int m
    );
    local_dot = Serial_dot(local_x, local_y, n_bar);
    MPI_Reduce(&local_dot, &dot, 1, MPI_FLOAT, 
        MPI_SUM, 0, MPI_COMM_WORLD);
    return dot;
}
int main(int argc, char* argv[]) {
    float local_x[V_SIZE];
    float local_y[V_SIZE];
    int n, n_bar;
    float dot;
    int p, rank;
    void read_vector(
        float local_v[],
        int n_bar,
        int p,
        int rank,
        int value
    );
    float Parallel_dot(
        float local_x[],
        float local_y[],
        int n_bar
    );
    MPI_Init(&argc, &argv);
    MPI_Comm_size(MPI_COMM_WORLD, &p);
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    if (rank == 0) {
        n = V_SIZE;
    }
    MPI_Bcast(&n, 1, MPI_INT, 0, MPI_COMM_WORLD);
    n_bar = n / p;
    read_vector(local_x, n_bar, p, rank, 1);
    read_vector(local_y, n_bar, p, rank, 1);
    dot = Parallel_dot(local_x, local_y, n_bar);
    if (rank == 0) printf("the result is %f\n", dot);
    return 0;
    MPI_Finalize();
}