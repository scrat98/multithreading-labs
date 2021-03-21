#include <iostream>
#include <omp.h>
#include <thread>
#include <random>

#define NUM_OF_THREADS 8
#define ARRAY_SIZE 10000000

int main() {
    long *arr = new long[ARRAY_SIZE];
    for (int i = 0; i < ARRAY_SIZE; i++) {
        arr[i] = random();
    }

    auto t_start = std::chrono::high_resolution_clock::now();
    long long sum = 0;
    for (int i = 0; i < ARRAY_SIZE; i++) {
        sum += arr[i];
    }
    auto t_end = std::chrono::high_resolution_clock::now();
    std::cout << std::chrono::duration<double, std::milli>(t_end - t_start).count() << std::endl;

    auto t2_start = std::chrono::high_resolution_clock::now();
    long sum2 = 0;
#pragma omp parallel num_threads(NUM_OF_THREADS) default(none) shared(arr, sum2)
    {
        long long cumulativeSum = 0;
#pragma omp for
        for (int i = 0; i < ARRAY_SIZE; i++) {
            cumulativeSum += arr[i];
        }
#pragma omp atomic
        sum2 += cumulativeSum;
    }
    auto t2_end = std::chrono::high_resolution_clock::now();
    std::cout << std::chrono::duration<double, std::milli>(t2_end - t2_start).count() << std::endl;

    std::cout << (sum == sum2) << std::endl;

    delete[] arr;
    return 0;
}
