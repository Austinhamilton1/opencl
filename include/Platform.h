#pragma once
#include <vector>
#include <memory>

#include <CL/cl.h>

#include "Logger.h"

class Platform : public Logger {
private:
    cl_platform_id id;

public:
    Platform(cl_platform_id id) : id(id) {};
    ~Platform() {};

    inline cl_platform_id getId() { return id; };

    static std::vector<Platform> allPlatforms();
    
    template <cl_platform_info S, typename T> std::shared_ptr<T[]> getInfo() {
        size_t param_size; //size of the parameter

        logInfo("Calling Platform::getInfo");

        //get size of parameter
        logInfo("Calling clGetPlatformInfo");
        result = clGetPlatformInfo(id, S, 0, nullptr, &param_size);
        logInfo("clGetPlatformInfo called");

        if(result != CL_SUCCESS) {
            logError("clGetPlatformInfo()");
            return nullptr;
        }

        //allocate the shared pointer for the paramter value
        logInfo("Creating shared pointer");
        std::shared_ptr<T[]> value(new T[param_size/sizeof(T)], std::default_delete<T[]>());
        logInfo("Shared pointer created");

        //get platform info
        logInfo("Calling clGetPlatformInfo");
        result = clGetPlatformInfo(id, S, param_size, value.get(), nullptr);
        logInfo("clGetPlatformInfo called");

        if(result != CL_SUCCESS) {
            logError("clGetPlatformInfo()");
            return nullptr;
        }

        logInfo("Platform::getInfo called");

        return value;
    };
};