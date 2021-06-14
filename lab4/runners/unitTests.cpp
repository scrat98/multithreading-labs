#include "../src/PrefixSumKernel.hpp"
#include "../src/DeviceProvider.hpp"
#include <iostream>
#include <random>
#include <iomanip>

float *getRandomArray(int size) {
    std::random_device rd;
    std::mt19937 gen(rd());
    std::uniform_real_distribution<float> dis(-10.0, 10.0);

    auto array = new float[size];
    for (int i = 0; i < size; i++) {
        array[i] = dis(gen);
    }
    return array;
}

float *calculatePrefixSum(const float *data, int size) {
    auto prefixSum = new float[size];
    float sum = 0;
    for (int i = 0; i < size; i++) {
        sum += data[i];
        prefixSum[i] = sum;
    }
    return prefixSum;
}

int main() {
    auto openCLDevices = DeviceProvider::getOpenCLDevices();
    auto devices = DeviceProvider::getDevices(openCLDevices);
    auto device = devices.front();
    auto calculator = PrefixSumKernel(&device);

    int arraySize = 1e5;
    auto array = getRandomArray(arraySize);

    float *prefixSum;
    auto sequentialExecutionTime = measureTimeMillis([&] {
        prefixSum = calculatePrefixSum(array, arraySize);
    });
    std::cout << "Sequential time (ms): " << sequentialExecutionTime << std::endl;

    calculator.runKernel(array, arraySize, [&](
            float *result,
            cl::Event *profilingEventStage1,
            cl::Event *profilingEventStage3,
            double executionTime
    ) {
        std::cout << "Execution time (ms): " << executionTime << std::endl;

        for (int i = 0; i < arraySize; i++) {
            auto expected = prefixSum[i];
            auto actual = result[i];
            if (std::abs(expected - actual) >= 0.001f) {
                std::ostringstream errorMsg;
                errorMsg << std::setprecision(8)
                         << "Expected " << expected << ", but was " << actual << std::endl;
                throw std::runtime_error(errorMsg.str());
            }
        }
    });

    delete[] array;
    delete[] prefixSum;
}