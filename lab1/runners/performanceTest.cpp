#include <cstdio>
#include <random>
#include "../src/utils.h"
#include "../src/DeterminantCalculator.hpp"

Matrix *getRandomMatrix(int matrixSize) {
    std::random_device rd;
    std::mt19937 gen(rd());
    std::uniform_real_distribution<float> dis(-10.0, 10.0);

    auto matrix = new std::vector<std::vector<float>>(matrixSize);
    for (int i = 0; i < matrixSize; ++i) {
        (*matrix)[i].resize(matrixSize);
        for (int j = 0; j < matrixSize; ++j) {
            (*matrix)[i][j] = dis(gen);
        }
    }

    return matrix;
}

int main() {
    std::vector<int> numberOfThreadsToTest = {0, 1, 2, 4, 6, 8, 16};

    for (int matrixSize = 10; matrixSize < 2500; matrixSize += 10) {
        auto matrix = getRandomMatrix(matrixSize);
        std::printf("Matrix size: %d\n", matrixSize);

        for (int numberOfThreads: numberOfThreadsToTest) {
            auto det = DeterminantCalculator::withOMP(numberOfThreads);
            auto elapsedTime = measureTimeMillis([&] { det->calculate(*matrix); });
            std::printf("Threads: %d, Time: %ld\n", numberOfThreads, elapsedTime);
        }

        delete matrix;
    }
}