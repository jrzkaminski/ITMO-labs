#include <cstdlib>
#include <stdio.h>
#include <omp.h>
#include <chrono>

using namespace std::chrono;

int main(int argc, char** argv) {
    int n = atoi(argv[1]);
    double* a = new double[n];
    for (long int i = 0; i < n; i++) { // random vector of n numbers
        a[i] = (double)rand() / RAND_MAX;
        // printf("a[%ld] = %f\n", i, a[i]);
    }    

    for (int m = 1; m <= 10; m++) { // m stands for the number of threads
        double max_value = a[0];
        auto time_start = high_resolution_clock::now(); // time recording starting point
        #pragma omp parallel for num_threads(m) reduction(max : max_value)
        // omp parameters: for cycle, number of threads, private variable max_number 
        for (int i = 0; i < n; i++) {
            max_value = max_value > a[i] ? max_value : a[i];
        }
        auto time_stop = high_resolution_clock::now(); // time recording stop point
        auto exectime = duration_cast<microseconds>(time_stop - time_start); // execution time calculation
        printf("Execution time via %d thread(s) in microseconds: %lld\n", m, exectime.count()); 
        printf("Maximum value: %f\n", max_value);
    }
    delete[] a;
    return 0;
}