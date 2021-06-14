#include "../src/DeviceProvider.hpp"
#include "../src/MatrixMultiplicatorKernel.hpp"
#include <iostream>
#include <random>
#include <cmath>
#include <sstream>
#include <iomanip>

void fillMatrix(float *matrix, int rows, int cols) {
    std::random_device rd;
    std::mt19937 gen(rd());
    std::uniform_real_distribution<float> dis(-10.0, 10.0);

    for (int i = 0; i < rows; i++) {
        for (int j = 0; j < cols; j++) {
            int index = i * cols + j;
            matrix[index] = dis(gen);
        }
    }
}

void multiplySimple(const float *matrixA, const float *matrixB, float *matrixC, int M, int N, int K) {
    for (int m = 0; m < M; m++) {
        for (int n = 0; n < N; n++) {
            float acc = 0.0f;
            for (int k = 0; k < K; k++) {
                acc += matrixA[K * m + k] * matrixB[N * k + n];
            }
            matrixC[N * m + n] = acc;
        }
    }
}

int main() {
    MultiplicatorKernelType kernelType = MultiplicatorKernelType::VECTORS;
    auto openCLDevices = DeviceProvider::getOpenCLDevices();
    auto devices = DeviceProvider::getDevices(openCLDevices);
    auto device = devices.front();
    auto calculator = MatrixMultiplicatorKernel(&device, kernelType);

    int M = 64;
    int N = 64;
    int K = 64;
    auto matrixA = new float[M * K];
    auto matrixB = new float[K * N];
    auto matrixC = new float[M * N];
    fillMatrix(matrixA, M, K);
    fillMatrix(matrixB, K, N);

//    calculator.runKernel(M, N, K, matrixA, matrixB, [&](
//            float *result, cl::Event *profilingEvent, long executionTime
//    ) {
//        auto start = profilingEvent->getProfilingInfo<CL_PROFILING_COMMAND_START>();
//        auto end = profilingEvent->getProfilingInfo<CL_PROFILING_COMMAND_END>();
//
//        std::cout << "Execution time (ms): " << executionTime << std::endl;
//        std::cout << "Kernel time (ms): " << (end - start) / 1000000 << std::endl;
//    });


    for (int m = 1; m < M; m++) {
        for (int n = 1; n < N; n++) {
            for (int k = 1; k < K; k++) {
                std::cout << "Checking with params: m = " << m << " n = " << n << " k = " << k << std::endl;
                multiplySimple(matrixA, matrixB, matrixC, m, n, k);

                calculator.runKernel(m, n, k, matrixA, matrixB, [&](
                        float *result, cl::Event *profilingEvent, long executionTime
                ) {
                    for (int i = 0; i < n * m; i++) {
                        auto expected = matrixC[i];
                        auto actual = result[i];
                        if (std::abs(expected - actual) >= 0.001f) {
                            std::ostringstream errorMsg;
                            errorMsg << std::setprecision(8)
                                     << "Expected " << expected << ", but was " << actual << std::endl;
                            throw std::runtime_error(errorMsg.str());
                        }
                    }
                });
            }
        }
    }

    delete[] matrixA;
    delete[] matrixB;
    delete[] matrixC;
}