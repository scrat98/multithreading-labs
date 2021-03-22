#include <iostream>
#include <omp.h>
#include <thread>
#include <random>
#include <functional>
#include <array>

#define ARRAY_SIZE 100000000
#define NUM_OF_THREADS 8

std::vector<long> *getRandomArray(int arraySize) {
    auto arr = new std::vector<long>(arraySize);
    for (int i = 0; i < arraySize; i++) {
        (*arr)[i] = random();
    }
    return arr;
}

long measureTimeMillis(const std::function<void()> &fun) {
    auto start = std::chrono::high_resolution_clock::now();
    fun();
    auto end = std::chrono::high_resolution_clock::now();
    return std::chrono::duration_cast<std::chrono::milliseconds>(end - start).count();
}

long long sumSequential(const std::vector<long> &arr) {
    long long sum = 0;
    for (long i : arr) {
        sum += i;
    }
    return sum;
}

long long sumParallel(const std::vector<long> &arr, int numOfThreads) {
    long long sum = 0;
#pragma omp parallel num_threads(numOfThreads) reduction (+:sum) default(none) shared(arr)
    {
#pragma omp for schedule(static)
        for (long i : arr) {
            sum += i;
        }
    }
    return sum;
}

int main() {
    auto arr = getRandomArray(ARRAY_SIZE);

    long long sumSequentialResult, sumParallelResult;
    std::cout << measureTimeMillis([&] { sumSequentialResult = sumSequential(*arr); }) << std::endl;
    std::cout << measureTimeMillis([&] { sumParallelResult = sumParallel(*arr, NUM_OF_THREADS); }) << std::endl;
    std::cout << (sumParallelResult == sumParallelResult ? "OK" : "FAIL");

    delete arr;
    return 0;
}