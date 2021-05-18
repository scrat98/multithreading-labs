#include "Runner.hpp"
#include "../src/DeviceProvider.hpp"
#include <cstdio>
#include <iostream>

int main(int argc, char *argv[]) {
    try {
        if (argc != 5) {
            throw std::invalid_argument(
                    "Wrong number of arguments. Must be in format <device number> <matrix in> <matrix out> <kernel type>"
            );
        }

        int deviceNumber = std::stoi(argv[1]);
        auto matrixInFile = std::string(argv[2]);
        auto matrixOutFile = std::string(argv[3]);
        int kernelTypeId = std::stoi(argv[4]);
        MultiplicatorKernelType kernelType;
        if (kernelTypeId == 0) {
            kernelType = MultiplicatorKernelType::NAIVE;
        } else if (kernelTypeId == 1) {
            kernelType = MultiplicatorKernelType::TILING;
        } else {
            throw std::runtime_error("Kernel type not found");
        }

        auto devices = DeviceProvider::getAll();
        if (devices.empty()) {
            throw std::runtime_error("No devices found");
        }

        std::cout << "Available devices: " << std::endl;
        for (auto &it : devices) {
            std::cout << it.getDeviceName() << std::endl;
        }
        std::cout << std::endl;

        Device *device;
        if (deviceNumber < 0 || deviceNumber >= devices.size()) {
            std::cout << "Device with number " << deviceNumber << " not exists. Using first device" << std::endl;
            device = &devices.front();
        } else {
            device = &devices[deviceNumber];
        }
        std::cout << "Selected device: " << device->getDeviceName() << std::endl;

        std::ifstream matrixIn(matrixInFile);
        if (!matrixIn.is_open()) {
            throw std::runtime_error("Could not open the matrix in file: " + matrixInFile);
        }

        std::ofstream matrixOut;
        matrixOut.open(matrixOutFile);
        if (!matrixOut.is_open()) {
            throw std::runtime_error("Could not open the matrix out file: " + matrixOutFile);
        }

        int m, n, k;
        matrixIn >> n >> k >> m;

        auto matrixA = new float[m * k];
        for (int i = 0; i < m * k; i++) {
            matrixIn >> matrixA[i];
        }

        auto matrixB = new float[k * n];
        for (int i = 0; i < k * n; i++) {
            matrixIn >> matrixB[i];
        }

        runKernel(*device, kernelType, m, n, k, matrixA, matrixB, [&](
                float *result, cl::Event *profilingEvent, long executionTime
        ) {
            matrixOut << n << ' ' << m << std::endl;
            for (int i = 0; i < m; i++) {
                for (int j = 0; j < n; j++) {
                    matrixOut << result[i * n + j] << ' ';
                }
                matrixOut << std::endl;
            }

            auto start = profilingEvent->getProfilingInfo<CL_PROFILING_COMMAND_START>();
            auto end = profilingEvent->getProfilingInfo<CL_PROFILING_COMMAND_END>();

            std::cout << "Execution time (ms): " << executionTime << std::endl;
            std::cout << "Kernel time (ms): " << (end - start) / 1000000 << std::endl;
        });
    } catch (const std::exception &e) {
        std::fprintf(stderr, "The program was aborted: %s", e.what());
        return 1;
    }
    return 0;
}