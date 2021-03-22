#include <cstdio>
#include <fstream>
#include <string>
#include "DeterminantCalculator.hpp"
#include "utils.h"

void run(const Matrix &matrix, int numOfThreads) {
    bool useOmp = numOfThreads != -1;

    DeterminantCalculator *detCalculator;
    if (useOmp) {
        if(numOfThreads < 0) {
            throw std::invalid_argument("Wrong number of threads. Must be -1(omp disabled), 0(default) or greater than.");
        }
        detCalculator = DeterminantCalculator::withOMP(numOfThreads);
    } else {
        detCalculator = DeterminantCalculator::withoutOMP();
    }

    float det;
    auto elapsedTime = measureTimeMillis([&] { det = detCalculator->calculate(matrix); });

    std::printf("Determinant: %f\n", det);
    std::printf("Time (%d thread(s)): %ldms\n", numOfThreads, elapsedTime);

    delete detCalculator;
}

int main(int argc, char *argv[]) {
    try {
        if (argc != 3) {
            throw std::invalid_argument("Wrong number of arguments. Must be in format <matrix file> <number of threads>");
        }

        auto matrixFileName = std::string(argv[1]);
        std::ifstream matrixFile(matrixFileName);
        int numOfThreads = atoi(argv[2]);
        if (!matrixFile.is_open()) {
            throw std::runtime_error("Could not open the matrix file: " + matrixFileName);
        }

        int matrixSize;
        matrixFile >> matrixSize;
        Matrix matrix(matrixSize);
        for (int i = 0; i < matrixSize; ++i) {
            matrix[i].resize(matrixSize);
            for (int j = 0; j < matrixSize; ++j) {
                matrixFile >> matrix[i][j];
            }
        }
        run(matrix, numOfThreads);

        matrixFile.close();
    } catch (const std::exception &e) {
        std::fprintf(stderr, "The program was aborted: %s", e.what());
        return 1;
    }
    return 0;
}