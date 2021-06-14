#ifndef MULTITHREADING_LABS_DEVICE_HPP
#define MULTITHREADING_LABS_DEVICE_HPP

#include <string>
#include "OpenCL.h"

enum DeviceType {
    GPU_DEDICATED,
    GPU_INTEGRATED,
    CPU,
    CUSTOM
};

class Device {
private:
    cl::Device *clDevice;
    std::string deviceName;
    DeviceType deviceType;

    static std::string getDeviceName(cl::Device *clDevice) {
        return clDevice->getInfo<CL_DEVICE_NAME>();
    }

    static DeviceType getDeviceType(cl::Device *clDevice) {
        auto deviceType = clDevice->getInfo<CL_DEVICE_TYPE>();
        auto t = clDevice->getInfo<CL_DEVICE_MAX_COMPUTE_UNITS>();
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

    const cl::Device *getCLDevice() const {
        return this->clDevice;
    }

    const std::string &getDeviceName() const {
        return this->deviceName;
    }

    const DeviceType &getDeviceType() const {
        return this->deviceType;
    }
};

#endif /* Device.hpp */
