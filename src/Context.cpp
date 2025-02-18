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

CommandQueue& Context::createQueue(Device& device) {
    logInfo("Calling Context::getQueue");

    logInfo("Calling clCreateCommandQueueWithProperties");
    cl_command_queue command_queue_id = clCreateCommandQueueWithProperties(context, device.getId(), nullptr, &result);
    logInfo("clCreateCommandQueueWithProperties called");
    
    //check result of queue creation
    if(result != CL_SUCCESS) {
        logError("clCreateCommandQueueWithProperties()");
        CommandQueue empty_queue({});
        queues.push_back(empty_queue);
    }
    else {
        //create the command queue object and add it to the queue map
        CommandQueue queue(command_queue_id);
        queues.push_back(queue);
        result = CL_SUCCESS;
    }

    logInfo("Context::getQueue called");

    return queues[queues.size() - 1];
}

Program& Context::createProgram(std::vector<std::string> source_files) {
    logInfo("Calling Context::createProgram");
    std::vector<const char *> sources;
    std::vector<size_t> sizes;
    for(unsigned int i = 0; i < source_files.size(); i++) {
        std::string source;
        //read the source file into the source of the program
        logInfo("Reading source file");
        std::ifstream input_file(source_files[i]);
        if(input_file.is_open()) {
            std::string line;
            //read the file line by line
            while(std::getline(input_file, line))
                source += line;
        }
        else {
            //log an error
            logError("Couldn't open file " + source_files[i]);
            result = -1;
            Program empty_program({});
            return empty_program;
        }
        logInfo("Source file read");
        sources.push_back(source.c_str());
        sizes.push_back(source.length());
    }

    //create the program
    logInfo("Calling clCreateProgramWithSource");
    cl_program program_id = clCreateProgramWithSource(context, sources.size(), sources.data(), sizes.data(), &result);
    logInfo("clCreateProgramWithSource called");

    //check for errors
    if(result != CL_SUCCESS) {
        logError("clCreateProgramWithSource()");
        Program empty_program({});
        return empty_program;
    }

    logInfo("Context::createProgram called");

    //return a program object
    Program program(program_id);
    return program;
}