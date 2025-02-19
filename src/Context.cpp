#include <fstream>

#include "Context.h"

Context::Context(std::vector<Device> devices) {
    logInfo("Calling Context::Context");

    //add devices to the Context
    logInfo("Adding devices");
    std::vector<cl_device_id> device_ids;
    for(unsigned int i = 0; i < devices.size(); i++) {
        this->devices.push_back(devices[i]);
        device_ids.push_back(devices[i].getId());
    }
    logInfo("Devices added");

    //create the context for the devices
    logInfo("Calling clCreateContext");
    context = clCreateContext(nullptr, device_ids.size(), device_ids.data(), nullptr, nullptr, &result);
    logInfo("clCreateContext called");

    //check for an error
    if(result != CL_SUCCESS)
        logError("clCreateContext()");                                                                            

    logInfo("Context::Context called");
};

Context::~Context() {
    logInfo("Calling clReleaseContext");
    clReleaseContext(context);
    logInfo("clReleaseContext called");
}