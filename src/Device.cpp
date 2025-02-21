#include "Device.h"

std::vector<cl::Device> cl::Device::allDevices(cl::Platform& platform) {
    int result; //device query device
    unsigned int num_devices; //number of devices
    std::vector<Device> all_devices;

    //get the number of devices
    if((result = clGetDeviceIDs(platform.getId(), CL_DEVICE_TYPE_ALL, 0, nullptr, &num_devices)) != CL_SUCCESS) 
        return all_devices;

    //allocate memory for devices
    std::vector<cl_device_id> all_device_ids(num_devices);

    //get the device ids
    if((result = clGetDeviceIDs(platform.getId(), CL_DEVICE_TYPE_ALL, num_devices, all_device_ids.data(), nullptr)) != CL_SUCCESS)
        return all_devices;

    //create a device object for each found device
    for(unsigned int i = 0; i < num_devices; i++)
        all_devices.push_back(Device(all_device_ids[i]));

    return all_devices;
}