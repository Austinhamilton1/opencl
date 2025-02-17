#pragma once

#include <vector>

#include <CL/cl.h>

#include "Logger.h"
#include "Platform.h"

class Device : public Logger {
private:
    cl_device_id id;

public:
    Device(cl_device_id id) : id(id) {};
    ~Device() {};

    inline cl_device_id getId() { return id; }

    static std::vector<Device> allDevices(Platform& platform);

    template <cl_device_info S, typename T> std::shared_ptr<T[]> getInfo() {
        std::string message;
        size_t param_size; //size of the parameter
        
        logInfo("Calling Device::getInfo");

        //get the size of the parameter
        logInfo("Calling clGetDeviceInfo");
        result = clGetDeviceInfo(id, S, 0, nullptr, &param_size);
        logInfo("clGetDeviceInfo called");

        if(result != CL_SUCCESS) {
            logError("clGetDeviceInfo()");
            return nullptr;
        }

        //allocate the shared pointer
        logInfo("Creating shared pointer");
        std::shared_ptr<T[]> value(new T[param_size/sizeof(T)], std::default_delete<T[]>());
        logInfo("Shared pointer created");

        //get device info
        logInfo("Calling clGetDeviceInfo");
        result = clGetDeviceInfo(id, S, param_size, value.get(), nullptr);
        logInfo("clGetDeviceInfo called");

        if(result != CL_SUCCESS) {
            logError("clGetDeviceInfo()");
            return nullptr;
        }

        logInfo("Device::getInfo called");

        return value;
    }
};