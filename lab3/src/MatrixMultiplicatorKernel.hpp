#ifndef MULTITHREADING_LABS_MATRIXMULTIPLICATORKERNEL_HPP
#define MULTITHREADING_LABS_MATRIXMULTIPLICATORKERNEL_HPP

#include "Device.hpp"
#include <string>
#include <vector>
#include <fstream>

enum class MultiplicatorKernelType {
    NAIVE,
};

class MatrixMultiplicatorKernel {
private:
    const Device &device;
    MultiplicatorKernelType kernelType;
    cl::Context context;
    cl::Program program;

    static std::string getKernelFile(MultiplicatorKernelType kernelType) {
        std::string kernelFileName;
        if (kernelType == MultiplicatorKernelType::NAIVE) {
            kernelFileName = "naive";
        }

        return "/home/ct/projects/multithreading-labs/lab3/src/kernels/" + kernelFileName + ".cl";
    }

public:

    explicit MatrixMultiplicatorKernel(const Device &device,
                                       MultiplicatorKernelType kernelType = MultiplicatorKernelType::NAIVE
    ) : device(device) {
        this->kernelType = kernelType;
        this->context = cl::Context(device.getCLDevice());

        std::ifstream sourceFile(getKernelFile(kernelType), std::ifstream::in | std::ifstream::binary);
        auto source = std::string(std::istreambuf_iterator<char>(sourceFile), std::istreambuf_iterator<char>());
        cl::Program::Sources sources({source});
        this->program = cl::Program(context, sources);
        program.build();
    }

    /*
     * m, n - result matrix dimension
     * k - common dimension of matrixA and matrixB
     */
    float *multiply(const float *matrixA, const float *matrixB, int m, int n, int k) const {
        auto matrixC = new float[m * n];

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

        cl::CommandQueue queue(context, device.getCLDevice());
        queue.enqueueWriteBuffer(buffer_a, CL_FALSE, 0, sizeof(float) * m * k, matrixA);
        queue.enqueueWriteBuffer(buffer_b, CL_FALSE, 0, sizeof(float) * k * n, matrixB);
        queue.enqueueNDRangeKernel(multiply_matrix, cl::NullRange, cl::NDRange(m, n), cl::NullRange);
        queue.enqueueReadBuffer(buffer_c, CL_TRUE, 0, sizeof(float) * m * n, matrixC);
        queue.finish();

        return matrixC;
    }
};

#endif /* MatrixMultiplicatorKernel.hpp */
