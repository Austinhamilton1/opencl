#ifndef DEVICE_H
#define DEVICE_H

#include <vector>

#include <CL/cl.h>

#include "Platform.h"

class Device {
private:
    cl_device_id id;
    int result;

public:
    Device(cl_device_id id) : id(id), result(CL_SUCCESS) {};
    ~Device() {};

    inline cl_device_id getId() { return id; }

    static std::vector<Device> allDevices(Platform& platform);

    template <cl_device_info S, typename T> std::shared_ptr<T[]> getInfo() {
        size_t param_size; //size of the parameter

        //get the size of the parameter
        if((result = clGetDeviceInfo(id, S, 0, nullptr, &param_size)) != CL_SUCCESS)
            return nullptr;

        //allocate the shared pointer
        std::shared_ptr<T[]> value(new T[param_size/sizeof(T)], std::default_delete<T[]>());
    
        //get device info
        if((result = clGetDeviceInfo(id, S, param_size, value.get(), nullptr)) != CL_SUCCESS)
            return nullptr;

        return value;
    }
};

#endif