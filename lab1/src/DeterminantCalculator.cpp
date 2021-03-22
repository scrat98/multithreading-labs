#include "DeterminantCalculator.hpp"

#include <cmath>

float DeterminantCalculator::calculate(Matrix matrix) const {
    int matrixSize = matrix.size();
    float det = 1;
    for (int i = 0; i < matrixSize; i++) {
        int maxPos = i;
        #pragma omp parallel if(useOmp) default(none) shared(matrix, maxPos, i, matrixSize) num_threads(numOfThreads)
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

        if (std::abs(matrix[maxPos][i]) < FLOAT_PRECISION) {
            det = 0;
            break;
        }

        if (i != maxPos) {
            std::swap(matrix[i], matrix[maxPos]);
            det *= -1;
        }
        det *= matrix[i][i];

        #pragma omp parallel for if(useOmp) default(none) schedule(static) shared(matrix, i, matrixSize) num_threads(numOfThreads)
        for (int j = i + 1; j < matrixSize; ++j) {
            float mul = -matrix[j][i] / matrix[i][i];
            for (int k = i; k < matrixSize; ++k) {
                matrix[j][k] += matrix[i][k] * mul;
            }
        }
    }

    return det;
}
