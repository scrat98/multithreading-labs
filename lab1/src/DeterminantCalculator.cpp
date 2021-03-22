#include "DeterminantCalculator.hpp"

#include <cmath>

DeterminantCalculator::~DeterminantCalculator() {

}

DeterminantCalculator::DeterminantCalculator(omp_sched_t scheduleType, int numOfThreads) {

}

float DeterminantCalculator::calculate(Matrix matrix) {
    int matrixSize = matrix.size();
    float det = 1;
    for (int i = 0; i < matrixSize; i++) {
        int maxPos = i;
        #pragma omp parallel default(none) shared(matrix, maxPos, i, matrixSize) num_threads(this->numOfThreads)
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

        if (std::abs(matrix[maxPos][i]) == 0.0) {
            det = 0;
            break;
        }

        if (i != maxPos) {
            std::swap(matrix[i], matrix[maxPos]);
            det *= -1;
        }
        det *= matrix[i][i];

        #pragma omp parallel for schedule(static) default(none) shared(matrix, i, matrixSize) num_threads(numOfThreads)
        for (int j = i + 1; j < matrixSize; ++j) {
            float mul = -matrix[j][i] / matrix[i][i];
            for (int k = i; k < matrixSize; ++k) {
                matrix[j][k] += matrix[i][k] * mul;
            }
        }
    }

    return det;
}