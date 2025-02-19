#pragma once
#include <memory>

#include <CL/cl.h>

#include "Logger.h"

class Buffer;

class Kernel : public Logger {
private:
    cl_kernel kernel;

public:
    Kernel(cl_kernel kernel) : kernel(kernel) {};
    ~Kernel();

    inline cl_kernel& getId() { return kernel; };

    void setArg(unsigned int index, Buffer& buffer);

    template <cl_kernel_info S, typename T> std::shared_ptr<T[]> getInfo() {
        std::string message;
        size_t param_size; //size of the parameter
        
        logInfo("Calling Kernel::getInfo");

        //get the size of the parameter
        logInfo("Calling clGetKernelInfo");
        result = clGetKernelInfo(kernel, S, 0, nullptr, &param_size);
        logInfo("clGetKernelInfo called");

        if(result != CL_SUCCESS) {
            logError("clGetKernelInfo()");
            return nullptr;
        }

        //allocate the shared pointer
        logInfo("Creating shared pointer");
        std::shared_ptr<T[]> value(new T[param_size/sizeof(T)], std::default_delete<T[]>());
        logInfo("Shared pointer created");

        //get kernel info
        logInfo("Calling clGetKernelInfo");
        result = clGetKernelInfo(kernel, S, param_size, value.get(), nullptr);
        logInfo("clGetKernelInfo called");

        if(result != CL_SUCCESS) {
            logError("clGetKernelInfo()");
            return nullptr;
        }

        logInfo("Kernel::getInfo called");

        return value;
    }
};