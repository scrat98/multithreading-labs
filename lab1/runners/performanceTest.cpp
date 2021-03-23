#include <cstdio>
#include <random>
#include <map>
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
    std::map<int, std::map<int, long long>> results;

    for (int matrixSize = 10; matrixSize < 1000; matrixSize += 10) {
        auto matrix = getRandomMatrix(matrixSize);
        std::printf("Matrix size: %d\n", matrixSize);

        for (int numberOfThreads: numberOfThreadsToTest) {
            auto det = DeterminantCalculator::withOMP(numberOfThreads);
            auto elapsedTime = measureTimeMillis([&] { det->calculate(*matrix); });
            std::printf("Threads: %d, Time: %ld\n", numberOfThreads, elapsedTime);
            results[matrixSize][numberOfThreads] = elapsedTime;
        }

        auto det = DeterminantCalculator::withoutOMP();
        auto elapsedTime = measureTimeMillis([&] { det->calculate(*matrix); });
        std::printf("Threads: -1, Time: %ld\n", elapsedTime);
        results[matrixSize][-1] = elapsedTime;

        delete matrix;
    }

    auto resFile = fopen("result.csv", "w");
    std::fprintf(resFile, "Matrix size");
    for (auto const &res: results.begin()->second) {
        std::fprintf(resFile, ", thread-<%d>", res.first);
    }
    std::fprintf(resFile, "\n");

    for (auto const&[matrixSize, result]: results) {
        std::fprintf(resFile, "%d", matrixSize);
        for (auto const &res: result) {
            std::fprintf(resFile, ", %lld", res.second);
        }
        std::fprintf(resFile, "\n");
    }

    return 0;
}