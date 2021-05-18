#ifndef MULTITHREADING_LABS_RUNNER_HPP
#define MULTITHREADING_LABS_RUNNER_HPP

#include "../src/MatrixMultiplicatorKernel.hpp"
#include "../src/utils.h"
#include <sstream>
#include <functional>
#include <exception>

void runKernel(const Device &device,
               MultiplicatorKernelType kernelType,
               int M, int N, int K,
               const float *matrixA, const float *matrixB,
               const std::function<void(float *result, cl::Event * profilingEvent, long executionTime)> &handler
) {
    try {
        MatrixMultiplicatorKernel calculator(device);
        std::pair<float *, cl::Event *> result;
        auto executionTime = measureTimeMillis([&] {
            result = calculator.multiply(matrixA, matrixB, M, N, K);
        });
        handler(result.first, result.second, executionTime);
    } catch (const cl::BuildError &e) {
        std::ostringstream buildErrorLog;
        buildErrorLog << "Build error: " << std::endl;
        for (auto &error: e.getBuildLog()) {
            buildErrorLog << error.second << std::endl;
        }
        throw std::runtime_error(buildErrorLog.str());
    }
    catch (const cl::Error &e) {
        auto clError = clGetErrorString(e.err());
        throw std::runtime_error("OpenCL error " + std::string(e.what()) + " " + clError);
    }
}

#endif /* Runner.hpp */
