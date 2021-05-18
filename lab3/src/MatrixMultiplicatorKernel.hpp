#ifndef MULTITHREADING_LABS_MATRIXMULTIPLICATORKERNEL_HPP
#define MULTITHREADING_LABS_MATRIXMULTIPLICATORKERNEL_HPP

#include "Device.hpp"
#include "utils.h"
#include <string>
#include <fstream>
#include <sstream>
#include <functional>
#include <exception>

enum class MultiplicatorKernelType {
    NAIVE,
    TILING
};

class MatrixMultiplicatorKernel {
private:
    /*
     * Warp size for devices:
     * Nvidia: 32
     * AMD: 32/64
     * Intel: variable
     *
     * Minimum CUDA cores (ALV) = 256
     *
     * We have 2 dimensions => local group dimension size = sqrt(256) = 16
     *
     * Example:
     * Nvidia GTX 1650
     * CUDA cores = 896 => Local-shared groups 896 / 32 = 28
     */
    static const int LOCAL_GROUP_DIMENSION_SIZE = 16;

    Device *device;
    MultiplicatorKernelType kernelType;

    /*
     * Will automatically release resources. See detail::Wrapper destructor
     */
    cl::Context context;
    cl::Program program;
    cl::CommandQueue queue;

    static std::string getKernelFile(MultiplicatorKernelType kernelType) {
        std::string kernelFileName;
        if (kernelType == MultiplicatorKernelType::NAIVE) {
            kernelFileName = "naive";
        } else if (kernelType == MultiplicatorKernelType::TILING) {
            kernelFileName = "tiling";
        }

        return "kernels/" + kernelFileName + ".cl";
    }

    static int roundUp(int numToRound, int multiple) {
        return ((numToRound + multiple - 1) / multiple) * multiple;
    }

    /*
     * m, n - result matrix dimension
     * k - common dimension of matrixA and matrixB
     */
    std::pair<float *, cl::Event *> multiply(const float *matrixA, const float *matrixB, int m, int n, int k) const {
        auto matrixC = new float[m * n];
        auto profilingEvent = new cl::Event;

        cl::Buffer buffer_a(context, CL_MEM_READ_ONLY | CL_MEM_HOST_WRITE_ONLY, sizeof(float) * m * k);
        cl::Buffer buffer_b(context, CL_MEM_READ_ONLY | CL_MEM_HOST_WRITE_ONLY, sizeof(float) * k * n);
        cl::Buffer buffer_c(context, CL_MEM_WRITE_ONLY | CL_MEM_HOST_READ_ONLY, sizeof(float) * m * n);

        cl::Kernel multiply_matrix(program, "multiply_matrix");
        multiply_matrix.setArg(0, m);
        multiply_matrix.setArg(1, n);
        multiply_matrix.setArg(2, k);
        multiply_matrix.setArg(3, buffer_a);
        multiply_matrix.setArg(4, buffer_b);
        multiply_matrix.setArg(5, buffer_c);

        queue.enqueueWriteBuffer(buffer_a, CL_FALSE, 0, sizeof(float) * m * k, matrixA);
        queue.enqueueWriteBuffer(buffer_b, CL_FALSE, 0, sizeof(float) * k * n, matrixB);
        queue.enqueueNDRangeKernel(multiply_matrix,
                                   cl::NullRange,
                                   cl::NDRange(
                                           roundUp(m, LOCAL_GROUP_DIMENSION_SIZE),
                                           roundUp(n, LOCAL_GROUP_DIMENSION_SIZE)
                                   ),
                                   cl::NDRange(LOCAL_GROUP_DIMENSION_SIZE, LOCAL_GROUP_DIMENSION_SIZE),
                                   NULL, profilingEvent
        );
        queue.enqueueReadBuffer(buffer_c, CL_TRUE, 0, sizeof(float) * m * n, matrixC);
        queue.finish();
        profilingEvent->wait();

        return std::make_pair(matrixC, profilingEvent);
    }

public:

    explicit MatrixMultiplicatorKernel(Device *device,
                                       MultiplicatorKernelType kernelType = MultiplicatorKernelType::TILING
    ) {
        this->device = device;
        this->kernelType = kernelType;
        this->context = cl::Context(*device->getCLDevice());

        auto kernelFleName = getKernelFile(kernelType);
        std::ifstream sourceFile(kernelFleName, std::ifstream::in | std::ifstream::binary);
        if (!sourceFile.is_open()) {
            throw std::runtime_error("Could not open kernel file: " + kernelFleName);
        }
        auto source = std::string(std::istreambuf_iterator<char>(sourceFile), std::istreambuf_iterator<char>());
        cl::Program::Sources sources({source});
        this->program = cl::Program(context, sources);
        program.build();

        this->queue = cl::CommandQueue(context, *device->getCLDevice(), CL_QUEUE_PROFILING_ENABLE);
    }

    void runKernel(int M, int N, int K,
                   const float *matrixA, const float *matrixB,
                   const std::function<void(float *result, cl::Event *profilingEvent, long executionTime)> &handler
    ) {
        try {
            std::pair<float *, cl::Event *> result;
            auto executionTime = measureTimeMillis([&] {
                result = this->multiply(matrixA, matrixB, M, N, K);
            });
            handler(result.first, result.second, executionTime);
            delete[] result.first;
            delete result.second;
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
};

#endif /* MatrixMultiplicatorKernel.hpp */
