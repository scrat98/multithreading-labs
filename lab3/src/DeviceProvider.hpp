#ifndef MULTITHREADING_LABS_DEVICE_PROVIDER_HPP
#define MULTITHREADING_LABS_DEVICE_PROVIDER_HPP

#include <vector>
#include <algorithm>
#include "Device.hpp"

class DeviceProvider {
public:
    static std::vector<cl::Device> getOpenCLDevices() {
        std::vector<cl::Device> devices;

        std::vector<cl::Platform> platforms;
        cl::Platform::get(&platforms);
        for (auto &platform: platforms) {
            std::vector<cl::Device> platform_devices;
            platform.getDevices(CL_DEVICE_TYPE_ALL, &platform_devices);
            std::move(platform_devices.begin(), platform_devices.end(), std::back_inserter(devices));
        }

        return devices;
    }

    static std::vector<Device> getDevices(std::vector<cl::Device> &openCLDevices) {
        std::vector<Device> devices;

        for (auto &it: openCLDevices) {
            devices.emplace_back(&it);
        }

        std::sort(devices.begin(), devices.end(), [](const Device &a, const Device &b) {
            return a.getDeviceType() < b.getDeviceType();
        });
        return devices;
    }
};

#endif /* DeviceProvider.hpp */
