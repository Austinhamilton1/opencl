#include <vector>
#include <iostream>
#include <string>

#include "Logger.h"
#include "Platform.h"
#include "Device.h"

int main() {
    std::vector<cl::Platform> all_platforms = cl::Platform::allPlatforms();

    if(all_platforms.size() == 0) {
        std::cout << "No platforms found" << std::endl;
        return 0;
    }

    std::cout << all_platforms.size() << " platform(s) found" << std::endl;

    cl::Platform default_platform = all_platforms[0];

    std::cout << "Platform name: " << default_platform.getInfo<CL_PLATFORM_NAME, char>() << std::endl;

    if(!default_platform.checkResult(CL_SUCCESS)) {
        std::cout << default_platform.getError();
        return -1;
    }

    std::vector<cl::Device> all_devices = cl::Device::allDevices(default_platform);

    if(all_devices.size() == 0) {
        std::cout << "No devices found" << std::endl;
        return 0;
    }

    cl::Device default_device = all_devices[0];
    std::cout << "\tDevice Name: " << default_device.getInfo<CL_DEVICE_NAME, char>() << std::endl;
    std::cout << "\tDevice Vendor: " << default_device.getInfo<CL_DEVICE_VENDOR, char>() << std::endl;
    std::cout << "\tDevice Version: " << default_device.getInfo<CL_DEVICE_VERSION, char>() << std::endl;

    if(!default_device.checkResult(CL_SUCCESS)) {
        std::cout << default_device.getError();
        return -1;
    }

    return 0;
}