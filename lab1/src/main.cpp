#include <iostream>
#include "DeterminantCalculator.hpp"
#include "utils.h"

int main() {
//    Matrix matrix = {
//            {1,   2,   8.8, 9.9},
//            {3.0, 4.0, 3.3, 4.4},
//            {2.0, 5.2, 2.2, 5.5},
//            {6.3, 7.8, 6.3, 7.8}
//    };
    srand(time(NULL));
    int matrixSize = 7;
    auto matrix = Matrix(matrixSize);
    for (int i = 0; i < matrixSize; ++i) {
        matrix[i].resize(matrixSize);
        for (int j = 0; j < matrixSize; ++j) {
            matrix[i][j] = 10.0 * rand() / RAND_MAX;
        }
    }
    for (int i = 0; i < matrix.size(); i++) {
        for (int j = 0; j < matrix.size(); j++) {
            std::cout << matrix[i][j] << ' ';
        }
        std::cout << std::endl;
    }
    auto detCalculator = DeterminantCalculator::withOMP(omp_sched_auto, 8);

    float det;
    auto elapsedTime = measureTimeMillis([&] { det = detCalculator->calculate(matrix); });

    std::cout << "Determinant: " << det << std::endl;
    std::cout << "Time (8 thread(s)): " << elapsedTime << "ms" << std::endl;

    delete detCalculator;

    return 0;
}