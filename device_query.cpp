#include <vector>
#include <iostream>

#include "Platform.h"
#include "Device.h"

int main() {
    std::vector<Platform> all_platforms = Platform::allPlatforms();

    if(all_platforms.size() == 0) {
        std::cout << "No platforms found" << std::endl;
        return 0;
    }

    std::cout << all_platforms.size() << " platform(s) found" << std::endl;

    Platform default_platform = all_platforms[0];

    std::cout << "Platform name: " << default_platform.getInfo<CL_PLATFORM_NAME, char>() << std::endl;

    std::vector<Device> all_devices = Device::allDevices(default_platform);

    if(all_devices.size() == 0) {
        std::cout << "No devices found" << std::endl;
        return 0;
    }

    for(unsigned int i = 0; i < all_devices.size(); i++) {
        std::cout << "Device " << i + 1 << ":" << std::endl;
        std::cout << "\tDevice Name: " << all_devices[i].getInfo<CL_DEVICE_NAME, char>() << std::endl;
        std::cout << "\tDevice Vendor: " << all_devices[i].getInfo<CL_DEVICE_VENDOR, char>() << std::endl;
        std::cout << "\tDevice Version: " << all_devices[i].getInfo<CL_DEVICE_VERSION, char>() << std::endl;
    }

    return 0;
}