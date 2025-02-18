#pragma once
#include <vector>
#include <string>
#include <memory>
#include <unordered_map>

#include <CL/cl.h>

#include "Logger.h"
#include "Device.h"
#include "Kernel.h"

class Program : public Logger {
private:
    cl_program program;
    std::vector<Device> devices;
    std::unordered_map<std::string, Kernel> kernels;

public:
    Program(cl_program program) : program(program) {};
    ~Program();

    inline void setDevices(std::vector<Device>& devices) { this->devices = devices; };

    void build();
    void addKernel(std::string name);
    Kernel& getKernel(std::string name);

    template <cl_context_info S, typename T> std::shared_ptr<T[]> getInfo() {
        std::string message;
        size_t param_size; //size of the parameter
        
        logInfo("Calling Program::getInfo");

        //get the size of the parameter
        logInfo("Calling clGetProgramInfo");
        result = clGetProgramInfo(program, S, 0, nullptr, &param_size);
        logInfo("clGetProgramInfo called");

        if(result != CL_SUCCESS) {
            logError("clGetProgramInfo()");
            return nullptr;
        }

        //allocate the shared pointer
        logInfo("Creating shared pointer");
        std::shared_ptr<T[]> value(new T[param_size/sizeof(T)], std::default_delete<T[]>());
        logInfo("Shared pointer created");

        //get program info
        logInfo("Calling clGetProgramInfo");
        result = clGetProgramInfo(program, S, param_size, value.get(), nullptr);
        logInfo("clGetProgramInfo called");

        if(result != CL_SUCCESS) {
            logError("clGetProgramInfo()");
            return nullptr;
        }

        logInfo("Program::getInfo called");

        return value;
    }
};