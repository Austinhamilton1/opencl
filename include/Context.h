#pragma once
#include <vector>
#include <memory>

#include <CL/cl.h>

#include "Logger.h"

class Context : public Logger {
private:
    cl_context context;

public:
    Context(cl_context context) : context(context) {};
    ~Context();

    template <cl_device_info S, typename T> std::shared_ptr<T[]> getInfo() {
        std::string message;
        size_t param_size; //size of the parameter
        
        logInfo("Calling Context::getInfo");

        //get the size of the parameter
        logInfo("Calling clGetContextInfo");
        result = clGetContextInfo(context, S, 0, nullptr, &param_size);
        logInfo("clGetContextInfo called");

        if(result != CL_SUCCESS) {
            logError("clGetContextInfo()");
            return nullptr;
        }

        //allocate the shared pointer
        logInfo("Creating shared pointer");
        std::shared_ptr<T[]> value(new T[param_size/sizeof(T)], std::default_delete<T[]>());
        logInfo("Shared pointer created");

        //get device info
        logInfo("Calling clGetContextInfo");
        result = clGetDeviceInfo(context, S, param_size, value.get(), nullptr);
        logInfo("clGetContextInfo called");

        if(result != CL_SUCCESS) {
            logError("clGetContextInfo()");
            return nullptr;
        }

        logInfo("Context::getInfo called");

        return value;
    }
};