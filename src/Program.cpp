#include <fstream>

#include "Program.h"
#include "Context.h"

cl::Program::Program(cl::Context& context, std::string source_file) {
    logInfo("Calling Program::Program");

    logInfo("Reading source file");
    std::ifstream input_file(source_file);
    if(input_file.is_open()) {
        std::string line;
        //read the file line by line
        while(std::getline(input_file, line)) {
            source += line;
            source += "\n";
        }
    }
    else {
        //log an error
        logError("Couldn't open file " + source_file);
        result = SOURCE_FILE_NOT_FOUND;
        return;
    }

    logInfo("Source file read");

    devices = context.getDevices();
    const char *data = source.c_str();

    //create the program
    logInfo("Calling clCreateProgramWithSource");
    program = clCreateProgramWithSource(context.getId(), 1, &data, nullptr, &result);
    logInfo("clCreateProgramWithSource called");

    //check for errors
    if(result != CL_SUCCESS) {
        logError("clCreateProgramWithSource()");
        return;
    }

    logInfo("Program::Program called");
}

cl::Program::~Program() {
    logInfo("Calling clReleaseProgram");
    if(program != nullptr)
        clReleaseProgram(program);
    logInfo("clReleaseProgram called");
}

void cl::Program::build() {
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

void cl::Program::addKernel(std::string name) {
    logInfo("Calling Program::addKernel");

    logInfo("Calling clCreateKernel");
    cl_kernel kernel_id = clCreateKernel(program, name.c_str(), &result);
    logInfo("clCreateKernel called");

    if(result != CL_SUCCESS) {
        logError("clCreateKernel()");
        return;
    }

    logInfo("Program::addKernel called");

    std::shared_ptr<Kernel> kernel = std::make_shared<Kernel>(kernel_id);
    kernels[name] = kernel;
}

std::shared_ptr<cl::Kernel> cl::Program::getKernel(std::string name) {
    logInfo("Calling Program::getKernel");
    logInfo("Program::getKernel called");
    return kernels[name];
}