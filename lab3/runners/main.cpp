#include "../src/DeviceProvider.hpp"

int main(int argc, char *argv[]) {
    try {
        auto devices = DeviceProvider::getAll();
        if (devices.empty()) {
            throw std::runtime_error("No devices found");
        }

        auto device = devices[0];
        auto context = "";

    } catch (const std::exception &e) {
        std::fprintf(stderr, "The program was aborted: %s", e.what());
        return 1;
    }

    return 0;
}