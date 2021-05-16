#ifndef MULTITHREADING_LABS_DEVICE_HPP
#define MULTITHREADING_LABS_DEVICE_HPP

#include <string>
#include "OpenCL.h"

enum DeviceType {
    CPU,
    GPU_INTEGRATED,
    GPU_DEDICATED,
    CUSTOM
};

class Device {
private:
    cl::Device *clDevice;
    std::string deviceName;
    DeviceType deviceType;

    static std::string getDeviceName(const cl::Device *clDevice) {
        return clDevice->getInfo<CL_DEVICE_NAME>();
    }

    static DeviceType getDeviceType(const cl::Device *clDevice) {
        auto deviceType = clDevice->getInfo<CL_DEVICE_TYPE>();
        if (deviceType == CL_DEVICE_TYPE_CPU) {
            return DeviceType::CPU;
        }

        if (deviceType == CL_DEVICE_TYPE_GPU) {
            auto isIntegrated = clDevice->getInfo<CL_DEVICE_HOST_UNIFIED_MEMORY>();
            if (isIntegrated == CL_TRUE) {
                return DeviceType::GPU_INTEGRATED;
            } else {
                return DeviceType::GPU_DEDICATED;
            }
        }

        return DeviceType::CUSTOM;
    }

public:
    explicit Device(cl::Device *clDevice) {
        this->clDevice = clDevice;
        this->deviceName = getDeviceName(clDevice);
        this->deviceType = getDeviceType(clDevice);
    }

    cl::Device *getCLDevice() {
        return this->clDevice;
    }

    std::string getDeviceName() {
        return this->deviceName;
    }

    DeviceType getDeviceType() {
        return this->deviceType;
    }
};

#endif /* Device.hpp */
