#include "../src/DeviceProvider.hpp"
#include "Runner.hpp"
#include <iostream>

int main() {
    int M = 2;
    int N = 2;
    int K = 3;
    float matrixA[] = {
            1, 2, 3,
            4, 5, 6
    };

    float matrixB[] = {
            1, 2,
            3, 4,
            5, 6
    };

    auto devices = DeviceProvider::getAll();
    auto device = devices[2];

    return runKernel(device, MultiplicatorKernelType::NAIVE, M, N, K, matrixA, matrixB, [&](float *result) {
        for (int i = 0; i < M; i++) {
            for (int j = 0; j < N; j++) {
                std::cout << result[N * i + j] << ' ';
            }
            std::cout << std::endl;
        }
    });
}