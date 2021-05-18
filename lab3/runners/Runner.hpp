#ifndef MULTITHREADING_LABS_RUNNER_HPP
#define MULTITHREADING_LABS_RUNNER_HPP

#include "../src/MatrixMultiplicatorKernel.hpp"
#include <sstream>
#include <functional>
#include <exception>
#include <cstdio>

int runKernel(const Device &device,
              MultiplicatorKernelType kernelType,
              int M, int N, int K,
              const float *matrixA, const float *matrixB,
              const std::function<void(float *result)> &handler
) {
    try {
        MatrixMultiplicatorKernel calculator(device);
        auto result = calculator.multiply(matrixA, matrixB, M, N, K);
        handler(result);
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
    catch (const std::exception &e) {
        std::fprintf(stderr, "The program was aborted: %s", e.what());
        return 1;
    }
    return 0;
}

#endif /* Runner.hpp */
