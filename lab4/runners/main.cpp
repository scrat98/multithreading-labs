#include "../src/DeviceProvider.hpp"
#include "../src/PrefixSumKernel.hpp"
#include <cstdio>
#include <iostream>

int main(int argc, char *argv[]) {
    try {
        if (argc != 4) {
            throw std::invalid_argument(
                    "Wrong number of arguments. Must be in format <device number> <array in> <array out>"
            );
        }

        int deviceNumber = std::stoi(argv[1]);
        auto arrayInFile = std::string(argv[2]);
        auto arrayOutFile = std::string(argv[3]);

        auto openCLDevices = DeviceProvider::getOpenCLDevices();
        auto devices = DeviceProvider::getDevices(openCLDevices);
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
        auto calculator = PrefixSumKernel(device);

        std::ifstream arrayIn(arrayInFile);
        if (!arrayIn.is_open()) {
            throw std::runtime_error("Could not open the array in file: " + arrayInFile);
        }

        std::ofstream arrayOut;
        arrayOut.open(arrayOutFile);
        if (!arrayOut.is_open()) {
            throw std::runtime_error("Could not open the array out file: " + arrayOutFile);
        }

        int n;
        arrayIn >> n;

        auto array = new float[n];
        for (int i = 0; i < n; i++) {
            arrayIn >> array[i];
        }

        calculator.runKernel(array, n, [&](
                float *result,
                cl::Event *profilingEventStage1,
                cl::Event *profilingEventStage3,
                double executionTime
        ) {
            for (int i = 0; i < n; i++) {
                arrayOut << result[i] << ' ';
            }

            auto startStage1 = profilingEventStage1->getProfilingInfo<CL_PROFILING_COMMAND_START>();
            auto endStage1 = profilingEventStage1->getProfilingInfo<CL_PROFILING_COMMAND_END>();
            auto kernelTimeStage1 = (endStage1 - startStage1) / 1000000;

            auto startStage3 = profilingEventStage3->getProfilingInfo<CL_PROFILING_COMMAND_START>();
            auto endStage3 = profilingEventStage3->getProfilingInfo<CL_PROFILING_COMMAND_END>();
            auto kernelTimeStage3 = (endStage3 - startStage3) / 1000000;

            std::cout << "Execution time (ms): " << executionTime << std::endl;
            std::cout << "Kernel time (ms): " << kernelTimeStage1 + kernelTimeStage3 << std::endl;
        });
    } catch (const std::exception &e) {
        std::fprintf(stderr, "The program was aborted: %s", e.what());
        return 1;
    }
    return 0;
}