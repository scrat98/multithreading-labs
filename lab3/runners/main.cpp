#include "Runner.hpp"
#include "../src/DeviceProvider.hpp"

int main(int argc, char *argv[]) {
    auto devices = DeviceProvider::getAll();
    if (devices.empty()) {
        throw std::runtime_error("No devices found");
    }

    // TODO
//    return runKernel([&](float *result) {
//        for (int i = 0; i < M; i++) {
//            for (int j = 0; j < N; j++) {
//                std::cout << result[N * i + j] << ' ';
//            }
//            std::cout << std::endl;
//        }
//    });
}