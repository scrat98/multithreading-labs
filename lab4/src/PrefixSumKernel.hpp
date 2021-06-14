#ifndef MULTITHREADING_LABS_PREFIXSUMKERNEL_HPP
#define MULTITHREADING_LABS_PREFIXSUMKERNEL_HPP

#include "Device.hpp"
#include "utils.h"
#include <string>
#include <fstream>
#include <sstream>
#include <functional>
#include <exception>
#include <iostream>

class PrefixSumKernel {
private:

    static const int BLOCKS_COUNT = 256;

    inline static const std::string KERNEL_FILE = "kernels/prefixSum.cl";

    Device *device;

    /*
     * Will automatically release resources. See detail::Wrapper destructor
     */
    bool haveInitialized = false;
    cl::Context context;
    cl::Program program;
    cl::CommandQueue queue;

    void initializeIfNeeded() {
        if (haveInitialized) return;

        this->context = cl::Context(*device->getCLDevice());

        std::ifstream sourceFile(KERNEL_FILE, std::ifstream::in | std::ifstream::binary);
        if (!sourceFile.is_open()) {
            throw std::runtime_error("Could not open kernel file: " + KERNEL_FILE);
        }
        auto source = std::string(std::istreambuf_iterator<char>(sourceFile), std::istreambuf_iterator<char>());
        cl::Program::Sources sources({source});
        this->program = cl::Program(context, sources);
        program.build();

        this->queue = cl::CommandQueue(context, *device->getCLDevice(), CL_QUEUE_PROFILING_ENABLE);

        haveInitialized = true;
    }

    void calculatePrefixSum(float *data, int size) const {
        for (int i = 1; i < size; i++) {
            data[i] = data[i - 1] + data[i];
        }
    }

    std::tuple<float *, cl::Event *, cl::Event *> prefixSum(float *data, int size) const {
        auto elementsPerBlock = size / BLOCKS_COUNT;
        if (size % BLOCKS_COUNT != 0) elementsPerBlock++;

        auto prefixSum = new float[size];
        auto profilingEventStage1 = new cl::Event;
        auto profilingEventStage3 = new cl::Event;

        // calculate prefix sum for blocks - stage1
        cl::Buffer buffer_data(context, CL_MEM_READ_ONLY | CL_MEM_HOST_WRITE_ONLY, sizeof(float) * size);
        cl::Buffer buffer_prefix_sum(context, CL_MEM_READ_WRITE | CL_MEM_HOST_READ_ONLY, sizeof(float) * size);

        cl::Kernel prefix_sum_stage1(program, "prefix_sum_stage1");
        prefix_sum_stage1.setArg(0, buffer_data);
        prefix_sum_stage1.setArg(1, buffer_prefix_sum);
        prefix_sum_stage1.setArg(2, size);
        prefix_sum_stage1.setArg(3, elementsPerBlock);

        queue.enqueueWriteBuffer(buffer_data, CL_FALSE, 0, sizeof(float) * size, data);
        queue.enqueueNDRangeKernel(prefix_sum_stage1,
                                   cl::NullRange,
                                   cl::NDRange(BLOCKS_COUNT),
                                   cl::NullRange,
                                   NULL, profilingEventStage1
        );
        queue.enqueueReadBuffer(buffer_prefix_sum, CL_TRUE, 0, sizeof(float) * size, prefixSum);
        profilingEventStage1->wait();

        // calculate suffix sum on lastElements of each block - stage2
        auto blocksSum = new float[BLOCKS_COUNT];
        for (int i = 0; i < BLOCKS_COUNT; i++) {
            int index = (i + 1) * elementsPerBlock - 1;
            if (index < size) {
                blocksSum[i] = prefixSum[index];
            } else {
                blocksSum[i] = 0;
            }
        }
        calculatePrefixSum(blocksSum, BLOCKS_COUNT);

        // stage3
        cl::Buffer buffer_blocks_sum(context, CL_MEM_READ_ONLY | CL_MEM_HOST_WRITE_ONLY, sizeof(float) * BLOCKS_COUNT);

        cl::Kernel prefix_sum_stage3(program, "prefix_sum_stage3");
        prefix_sum_stage3.setArg(0, buffer_prefix_sum);
        prefix_sum_stage3.setArg(1, buffer_blocks_sum);
        prefix_sum_stage3.setArg(2, size);
        prefix_sum_stage3.setArg(3, elementsPerBlock);

        queue.enqueueWriteBuffer(buffer_blocks_sum, CL_FALSE, 0, sizeof(float) * BLOCKS_COUNT, blocksSum);
        queue.enqueueNDRangeKernel(prefix_sum_stage3,
                                   cl::NullRange,
                                   cl::NDRange(BLOCKS_COUNT),
                                   cl::NullRange,
                                   NULL, profilingEventStage3
        );
        queue.enqueueReadBuffer(buffer_prefix_sum, CL_TRUE, 0, sizeof(float) * size, prefixSum);
        profilingEventStage3->wait();
        queue.finish();

        return std::make_tuple(prefixSum, profilingEventStage1, profilingEventStage3);
    }

public:

    explicit PrefixSumKernel(Device *device) {
        this->device = device;
    }

    void runKernel(float *data, int size,
                   const std::function<void(float *result,
                                            cl::Event *profilingEventStage1,
                                            cl::Event *profilingEventStage3,
                                            double executionTime)> &handler
    ) {
        try {
            this->initializeIfNeeded();
            std::tuple<float *, cl::Event *, cl::Event *> result;
            auto executionTime = measureTimeMillis([&] {
                result = this->prefixSum(data, size);
            });
            handler(std::get<0>(result), std::get<1>(result), std::get<2>(result), executionTime);
            delete[] std::get<0>(result);
            delete std::get<1>(result);
            delete std::get<2>(result);
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

#endif /* PrefixSumKernel.hpp */
