#pragma once
#include <vector>
#include <string>
#include <memory>
#include <unordered_map>

#include <CL/cl.h>

#include "Logger.h"
#include "Kernel.h"
#include "Device.h"

class Context;

class Program : public Logger {
private:
    cl_program program;
    std::vector<Device> devices;
    std::string source;
    std::unordered_map<std::string, std::shared_ptr<Kernel>> kernels;

public:
    Program(Context& context, std::string source_file);
    ~Program();

    inline void setDevices(std::vector<Device>& devices) { this->devices = devices; };
    inline std::string getSource() { return source; };

    void build();
    void addKernel(std::string name);
    std::shared_ptr<Kernel> getKernel(std::string name);

    template <cl_program_info S, typename T> std::shared_ptr<T[]> getInfo() {
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
    };

    template <cl_program_build_info S, typename T> std::shared_ptr<T[]> getInfo(Device& device){
        std::string message;
        size_t param_size; //size of the parameter
        
        logInfo("Calling Program::getInfo");

        //get the size of the parameter
        logInfo("Calling clGetProgramBuildInfo");
        result = clGetProgramBuildInfo(program, device.getId(), S, 0, nullptr, &param_size);
        logInfo("clGetProgramBuildInfo called");

        if(result != CL_SUCCESS) {
            logError("clGetProgramBuildInfo()");
            return nullptr;
        }

        //allocate the shared pointer
        logInfo("Creating shared pointer");
        std::shared_ptr<T[]> value(new T[param_size/sizeof(T)], std::default_delete<T[]>());
        logInfo("Shared pointer created");

        //get program info
        logInfo("Calling clGetProgramBuildInfo");
        result = clGetProgramBuildInfo(program, device.getId(), S, param_size, value.get(), nullptr);
        logInfo("clGetProgramBuildInfo called");

        if(result != CL_SUCCESS) {
            logError("clGetProgramBuildInfo()");
            return nullptr;
        }

        logInfo("Program::getInfo called");

        return value;
    };
};