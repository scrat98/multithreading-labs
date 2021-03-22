#include <iostream>

//// sum of array
//std::vector<long> *getRandomArray(int arraySize) {
//    auto arr = new std::vector<long>(arraySize);
//    for (int i = 0; i < arraySize; i++) {
//        (*arr)[i] = rand();
//    }
//    return arr;
//}
//
//long long sumSequential(const std::vector<long> &arr) {
//    long long sum = 0;
//    for (long i : arr) {
//        sum += i;
//    }
//    return sum;
//}
//
//long long sumParallel(const std::vector<long> &arr, int numOfThreads) {
//    long long sum = 0;
//#pragma omp parallel num_threads(numOfThreads) reduction (+:sum) default(none) shared(arr)
//    {
//#pragma omp for schedule(static)
//        for (long i : arr) {
//            sum += i;
//        }
//    }
//    return sum;
//}
//
//void testSum() {
//    auto arr = getRandomArray(ARRAY_SIZE);
//
//    long long sumSequentialResult, sumParallelResult;
//    std::cout << measureTimeMillis([&] { sumSequentialResult = sumSequential(*arr); }) << std::endl;
//    std::cout << measureTimeMillis([&] { sumParallelResult = sumParallel(*arr, NUM_OF_THREADS); }) << std::endl;
//    std::cout << (sumParallelResult == sumParallelResult ? "OK" : "FAIL");
//
//    delete arr;
//}

//void testDeterminantOfMatrix() {
//    auto matrix = getRandomMatrix(MATRIX_SIZE);
//
//    float determinantSequentialResult, determinantParallelResult;
//    std::cout << measureTimeMillis([&] {
//        determinantSequentialResult = determinantSequential(*matrix);
//    }) << std::endl;
//    std::cout << measureTimeMillis([&] {
//        determinantParallelResult = determinantParallel(*matrix, NUM_OF_THREADS);
//    }) << std::endl;
//    std::cout << (determinantSequentialResult == determinantParallelResult ? "OK" : "FAIL");
//
//    delete matrix;
//}


//std::vector<std::vector<float>> *getRandomMatrix(int matrixSize) {
////    auto matrix = new std::vector<std::vector<float>>(4);
////    (*matrix)[0].push_back(1.0);
////    (*matrix)[0].push_back(2);
////    (*matrix)[0].push_back(8.8);
////    (*matrix)[0].push_back(9.9);
////
////    (*matrix)[1].push_back(3.0);
////    (*matrix)[1].push_back(4.0);
////    (*matrix)[1].push_back(3.3);
////    (*matrix)[1].push_back(4.4);
////
////    (*matrix)[2].push_back(2.0);
////    (*matrix)[2].push_back(5.2);
////    (*matrix)[2].push_back(2.2);
////    (*matrix)[2].push_back(5.5);
////
////    (*matrix)[3].push_back(6.3);
////    (*matrix)[3].push_back(7.8);
////    (*matrix)[3].push_back(6.3);
////    (*matrix)[3].push_back(7.8);
//
////    (*matrix)[0].push_back(0.0);
////    (*matrix)[0].push_back(0);
////    (*matrix)[0].push_back(0);
////    (*matrix)[0].push_back(0);
////
////    (*matrix)[1].push_back(3.0);
////    (*matrix)[1].push_back(4.0);
////    (*matrix)[1].push_back(3.3);
////    (*matrix)[1].push_back(4.4);
////
////    (*matrix)[2].push_back(2.0);
////    (*matrix)[2].push_back(5.2);
////    (*matrix)[2].push_back(2.2);
////    (*matrix)[2].push_back(5.5);
////
////    (*matrix)[3].push_back(6.3);
////    (*matrix)[3].push_back(7.8);
////    (*matrix)[3].push_back(6.3);
////    (*matrix)[3].push_back(7.8);
//
//    auto matrix = new std::vector<std::vector<float>>(matrixSize);
//    for (int i = 0; i < matrixSize; ++i) {
//        (*matrix)[i].resize(matrixSize);
//        for (int j = 0; j < matrixSize; ++j) {
//            (*matrix)[i][j] = 100.0 * rand() / RAND_MAX;
//        }
//    }
//    return matrix;
//}

//float determinantSequential(std::vector<std::vector<float>> &matrix) {
//    int matrixSize = matrix.size();
//    float det = 1;
//    for (int i = 0; i < matrixSize; i++) {
//        int maxPos = i;
//        for (int j = i + 1; j < matrixSize; j++) {
//            if (std::abs(matrix[j][i]) > std::abs(matrix[maxPos][i])) {
//                maxPos = j;
//            }
//        }
//
//        if (abs(matrix[maxPos][i]) == 0.0) {
//            det = 0;
//            break;
//        }
//
//        if (i != maxPos) {
//            std::swap(matrix[i], matrix[maxPos]);
//            det *= -1;
//        }
//        det *= matrix[i][i];
//
//        for (int j = i + 1; j < matrixSize; ++j) {
//            float mul = -matrix[j][i] / matrix[i][i];
//            for (int k = i; k < matrixSize; ++k) {
//                matrix[j][k] += matrix[i][k] * mul;
//            }
//        }
//    }
//
//    return det;
//}

int main() {
//    testSum();
//    testDeterminantOfMatrix();
    return 0;
}