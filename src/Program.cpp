#include <fstream>

#include "Program.h"

Program::~Program() {
    clReleaseProgram(program);
}

void Program::build() {
    logInfo("Calling Program::build");

    std::vector<cl_device_id> device_ids;
    for(unsigned int i = 0; i < devices.size(); i++) 
        device_ids.push_back(devices[i].getId());

    logInfo("Calling clBuildProgram");
    result = clBuildProgram(program, device_ids.size(), device_ids.data(), nullptr, nullptr, nullptr);
    logInfo("clBuildProgram called");

    if(result != CL_SUCCESS) {
        logError("clBuildProgram()");
        return;
    }

    logInfo("Program::build called");
}

void Program::addKernel(std::string name) {
    logInfo("Calling Program::addKernel");

    logInfo("Calling clCreateKernel");
    cl_kernel kernel_id = clCreateKernel(program, name.c_str(), &result);
    logInfo("clCreateKernel called");

    if(result != CL_SUCCESS) {
        logError("clCreateKernel()");
        Kernel empty_kernel({});
        kernels[name] = empty_kernel;
    }

    logInfo("Program::addKernel called");

    Kernel kernel(kernel_id);
    kernels[name] = kernel;
}

Kernel& Program::getKernel(std::string name) {
    logInfo("Calling Program::getKernel");
    logInfo("Program::getKernel called");
    return kernels[name];
}