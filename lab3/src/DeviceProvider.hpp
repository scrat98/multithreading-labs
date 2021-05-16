#ifndef MULTITHREADING_LABS_DEVICE_PROVIDER_HPP
#define MULTITHREADING_LABS_DEVICE_PROVIDER_HPP

#include <vector>
#include "Device.hpp"

class DeviceProvider {
private:
    static std::vector<cl::Device> getOpenCLDevices() {
        std::vector<cl::Device> devices;

        std::vector<cl::Platform> platforms;
        cl::Platform::get(&platforms);
        for (auto &platform: platforms) {
            std::vector<cl::Device> platform_devices;
            platform.getDevices(CL_DEVICE_TYPE_ALL, &platform_devices);
            devices.insert(devices.end(), platform_devices.begin(), platform_devices.end());
        }

        return devices;
    }

public:
    static std::vector<Device> getAll() {
        std::vector<Device> devices;

        auto openCLDevices = getOpenCLDevices();
        for (auto &it: openCLDevices) {
            devices.emplace_back(&it);
        }

        return devices;
    }
};

#endif /* DeviceProvider.hpp */
