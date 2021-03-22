#include <iostream>
#include <omp.h>
#include <chrono>
#include <functional>
#include <vector>
#include <stdlib.h>
#include <cmath>
#include <utility>

#define ARRAY_SIZE 100000000
#define MATRIX_SIZE 1000
#define NUM_OF_THREADS 8

long measureTimeMillis(const std::function<void()> &fun) {
    auto start = std::chrono::high_resolution_clock::now();
    fun();
    auto end = std::chrono::high_resolution_clock::now();
    return std::chrono::duration_cast<std::chrono::milliseconds>(end - start).count();
}

// sum of array
std::vector<long> *getRandomArray(int arraySize) {
    auto arr = new std::vector<long>(arraySize);
    for (int i = 0; i < arraySize; i++) {
        (*arr)[i] = rand();
    }
    return arr;
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

void testSum() {
    auto arr = getRandomArray(ARRAY_SIZE);

    long long sumSequentialResult, sumParallelResult;
    std::cout << measureTimeMillis([&] { sumSequentialResult = sumSequential(*arr); }) << std::endl;
    std::cout << measureTimeMillis([&] { sumParallelResult = sumParallel(*arr, NUM_OF_THREADS); }) << std::endl;
    std::cout << (sumParallelResult == sumParallelResult ? "OK" : "FAIL");

    delete arr;
}

// determinant of matrix
std::vector<std::vector<float>> *getRandomMatrix(int matrixSize) {
    auto matrix = new std::vector<std::vector<float>>(matrixSize);
    for (int i = 0; i < matrixSize; ++i) {
        (*matrix)[i].resize(matrixSize);
        for (int j = 0; j < matrixSize; ++j) {
            (*matrix)[i][j] = rand();
        }
    }
    return matrix;
}


float determinantSequential(std::vector<std::vector<float>> &matrix) {
    int matrixSize = matrix.size();
    float det = 1;
    for (int i = 0; i < matrixSize; i++) {
        int maxPos = i;
        for (int j = i + 1; j < matrixSize; j++) {
            if (std::abs(matrix[j][i]) > std::abs(matrix[maxPos][i])) {
                maxPos = j;
            }
        }

        if (abs(matrix[maxPos][i]) == 0.0) {
            det = 0;
            break;
        }

        if (i != maxPos) {
            std::swap(matrix[i], matrix[maxPos]);
            det *= -1;
        }
        det *= matrix[i][i];

        for (int j = i + 1; j < matrixSize; ++j) {
            float mul = -matrix[j][i] / matrix[i][i];
            for (int k = i; k < matrixSize; ++k) {
                matrix[j][k] += matrix[i][k] * mul;
            }
        }
    }

    return det;
}

float determinantParallel(std::vector<std::vector<float>> &matrix, int numOfThreads) {
    int matrixSize = matrix.size();
    float det = 1;
    for (int i = 0; i < matrixSize; i++) {
        int maxPos = i;
#pragma omp parallel default(none) shared(matrix, maxPos, i, matrixSize) num_threads(numOfThreads)
        {
            int localMaxPos = maxPos;
#pragma omp for schedule(static)
            for (int j = i + 1; j < matrixSize; j++) {
                if (std::abs(matrix[j][i]) > std::abs(matrix[localMaxPos][i])) {
                    localMaxPos = j;
                }
            }
#pragma omp critical
            {
                if (std::abs(matrix[localMaxPos][i]) > std::abs(matrix[maxPos][i])) {
                    maxPos = localMaxPos;
                }
            }
        }

        if (abs(matrix[maxPos][i]) == 0.0) {
            det = 0;
            break;
        }

        if (i != maxPos) {
            std::swap(matrix[i], matrix[maxPos]);
            det *= -1;
        }
        det *= matrix[i][i];

#pragma omp parallel for default(none) shared(matrix, i, matrixSize) num_threads(numOfThreads)
        for (int j = i + 1; j < matrixSize; ++j) {
            float mul = -matrix[j][i] / matrix[i][i];
            for (int k = i; k < matrixSize; ++k) {
                matrix[j][k] += matrix[i][k] * mul;
            }
        }
    }

    return det;
}

void testDeterminantOfMatrix() {
    auto matrix = getRandomMatrix(MATRIX_SIZE);

    float determinantSequentialResult, determinantParallelResult;
    std::cout << measureTimeMillis([&] {
        determinantSequentialResult = determinantSequential(*matrix);
    }) << std::endl;
    std::cout << measureTimeMillis([&] {
        determinantParallelResult = determinantParallel(*matrix, NUM_OF_THREADS);
    }) << std::endl;
    std::cout << (determinantSequentialResult == determinantParallelResult ? "OK" : "FAIL");

    delete matrix;
}

int main() {
//    testSum();
    testDeterminantOfMatrix();
    return 0;
}