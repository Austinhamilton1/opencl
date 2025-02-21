#include <vector>
#include <iostream>
#include <memory>
#include <chrono>

#include <CL/cl.h>

#include "Platform.h"
#include "Device.h"
#include "Context.h"
#include "CommandQueue.h"
#include "Buffer.h"
#include "Program.h"
#include "Kernel.h"

#define SIZE 4096

int main() {
    //get available platforms
    std::vector<cl::Platform> all_platforms = cl::Platform::allPlatforms();
    if(all_platforms.size() == 0) {
        std::cout << "No platforms" << std::endl;
        return 0;
    }

    //get the deafult platform
    cl::Platform default_platform = all_platforms[0];
    std::cout << "Using platform: " << default_platform.getInfo<CL_PLATFORM_NAME, char>() << std::endl;

    //get available devices
    std::vector<cl::Device> all_devices = cl::Device::allDevices(default_platform);
    if(all_devices.size() == 0) {
        std::cout << "No devices" << std::endl;
        return 0;
    }

    //get the default device
    cl::Device default_device = all_devices[0];
    std::cout << "Using device: " << default_device.getInfo<CL_DEVICE_NAME, char>() << std::endl;

    //create the context for the devices
    cl::Context context(all_devices);
    std::cout << "Created a context with " << *(context.getInfo<CL_CONTEXT_NUM_DEVICES, int>().get()) << " devices" << std::endl;

    //create the command queue
    cl::CommandQueue queue(context, default_device);
    std::cout << "Created a command queue with " << *(queue.getInfo<CL_QUEUE_REFERENCE_COUNT, int>().get()) << " reference counts" << std::endl;

    //allocate the memory on the host
    int A_h[SIZE], B_h[SIZE], C_h[SIZE];
    for(int i = 0; i < SIZE; i++) {
        A_h[i] = i;
        B_h[i] = i + i;
        C_h[i] = 0;
    }
    std::cout << "Host memory allocated" << std::endl;

    //create the buffers for the kernel
    std::cout << "Creating buffers" << std::endl;
    cl::Buffer aBuffer(context, CL_MEM_READ_ONLY, SIZE * sizeof(int));
    cl::Buffer bBuffer(context, CL_MEM_READ_ONLY, SIZE * sizeof(int));
    cl::Buffer cBuffer(context, CL_MEM_WRITE_ONLY, SIZE * sizeof(int));
    std::cout << "Buffers created" << std::endl;

    //write the data to the buffers
    std::cout << "Writing data to buffers" << std::endl;
    
    queue.writeBuffer(aBuffer, true, 0, A_h);
    if(!queue.checkResult(CL_SUCCESS)) {
        std::cout << "Could not write to buffer A";
        return -1;
    }

    queue.writeBuffer(bBuffer, true, 0, B_h);
    if(!queue.checkResult(CL_SUCCESS)) {
        std::cout << "Could not write to buffer B";
        return -1;
    }

    std::cout << "Data written to buffers" << std::endl;

    //create the program
    std::cout << "Creating program" << std::endl;
    cl::Program main(context, "kernels/vadd.cl");
    if(!context.checkResult(CL_SUCCESS)) {
        std::cout << "Could not create program" << std::endl;
        return -1;
    }
    std::cout << "Program created" << std::endl;

    //build the program
    std::cout << "Building program" << std::endl;
    main.build();
    if(!main.checkResult(CL_SUCCESS)) {
        std::cout << "Could not build program" << std::endl;
        std::cout << main.getInfo<CL_PROGRAM_BUILD_LOG, char>(default_device) << std::endl;
        return -1;
    }
    std::cout << "Program built" << std::endl;

    //create kernel
    std::cout << "Creating vadd kernel" << std::endl;
    main.addKernel("vadd");
    if(!main.checkResult(CL_SUCCESS)) {
        std::cout << "Could not create vadd kernel" << std::endl;
        return -1;
    }
    std::cout << "Created vadd kernel" << std::endl;

    //set arguments
    std::cout << "Setting arguments" << std::endl;
    std::shared_ptr<cl::Kernel> vadd = main.getKernel("vadd");
    
    //set first argument
    vadd->setArg(0, aBuffer);
    if(!vadd->checkResult(CL_SUCCESS)) {
        std::cout << "Could not set first argument" << std::endl;
        return -1;
    }

    //set second argument
    vadd->setArg(1, bBuffer);
    if(!vadd->checkResult(CL_SUCCESS)) {
        std::cout << "Could not set second argument" << std::endl;
        return -1;
    }

    //set third argument
    vadd->setArg(2, cBuffer);
    if(!vadd->checkResult(CL_SUCCESS)) {
        std::cout << "Could not set third argument" << std::endl;
        return -1;
    }

    std::cout << "Arguments set" << std::endl;


    //run the kernel and time it
    std::cout << "Running kernel" << std::endl;
    queue.runKernel<4096, 1>(vadd);
    if(!queue.checkResult(CL_SUCCESS)) {
        std::cout << "Could not run kernel" << std::endl;
        return -1;
    }
    std::cout << "Kernel run" << std::endl;

    //copy data back from device
    std::cout << "Copying writing data from buffer" << std::endl;
    queue.readBuffer(cBuffer, true, 0, C_h);
    if(!queue.checkResult(CL_SUCCESS)) {
        std::cout << "Could not copy data from buffer C" << std::endl;
        return -1;
    }
    std::cout << "Data copied from buffer" << std::endl;


    //print out the results
    std::cout << "C_h: [";
    for(int i = 0; i < SIZE - 1; i++) {
        std::cout << C_h[i] << ", ";
    }
    std::cout << C_h[SIZE-1] << "]" << std::endl;

    return 0;
}