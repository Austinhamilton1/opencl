#ifndef PLATFORM_H
#define PLATFORM_H

#include <vector>
#include <memory>

#include <CL/cl.h>

class Platform {
private:
    cl_platform_id id;
    int result;

public:
    Platform(cl_platform_id id) : id(id), result(CL_SUCCESS) {};
    ~Platform() {};

    inline cl_platform_id getId() { return id; };

    static std::vector<Platform> allPlatforms();
    
    template <cl_platform_info S, typename T> std::shared_ptr<T[]> getInfo() {
        size_t param_size; //size of the parameter

        //get size of parameter
        if((result = clGetPlatformInfo(id, S, 0, nullptr, &param_size)) != CL_SUCCESS)
            return nullptr;

        //allocate the shared pointer for the paramter value
        std::shared_ptr<T[]> value(new T[param_size/sizeof(T)], std::default_delete<T[]>());

        //get platform info
        if((result = clGetPlatformInfo(id, S, param_size, value.get(), nullptr)) != CL_SUCCESS)
            return nullptr;

        return value;
    };
};

#endif