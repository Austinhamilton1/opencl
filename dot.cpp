#include <iostream>
#include <vector>
#include <memory>
#include <random>
#include <ctime>

#include <CL/cl.h>

#include "Platform.h"
#include "Device.h"
#include "Context.h"
#include "CommandQueue.h"
#include "Buffer.h"
#include "Program.h"
#include "Kernel.h"

#define SIZE 262144
#define THREADS 4096

int reduce(uint n, int *v) {
    int result = 0;
    for(uint i = 0; i < n; i++) {
        result += v[i];
    }
    return result;
}

int dot(uint n, int *a, int *b) {
    int dot_result = 0;
    for(uint i = 0; i < n; i++) {
        dot_result += a[i] * b[i];
    }
    return dot_result;
}

int main() {
    //allocate memory on the host
    int A_h[SIZE], B_h[SIZE], C_h[THREADS];
    uint len = SIZE;
    int dot_result_h, dot_result_d;
    std::mt19937 generator(std::time(0));
    std::uniform_int_distribution<int> distribution(-4096, 4096);

    //initialize memory on host
    for(uint i = 0; i < SIZE; i++) {
        A_h[i] = distribution(generator);
        B_h[i] = distribution(generator);
    }

    //get the dot result using the CPU 
    dot_result_h = dot(SIZE, A_h, B_h);
    std::cout << "Dot result on CPU: " << dot_result_h << std::endl;

    //get available platforms
    std::vector<Platform> all_platforms = Platform::allPlatforms();
    if(all_platforms.size() == 0) {
        std::cout << "No platforms available" << std::endl;
        return 0;
    }

    //get default platform
    Platform default_platform = all_platforms[0];
    std::cout << "Using platform: " << default_platform.getInfo<CL_PLATFORM_NAME, char>() << std::endl;

    //get available devices
    std::vector<Device> all_devices = Device::allDevices(default_platform);
    if(all_devices.size() == 0) {
        std::cout << "No devices available" << std::endl;
        return 0;
    }

    //get default device
    Device default_device = all_devices[0];
    std::cout << "Using device: " << default_device.getInfo<CL_DEVICE_NAME, char>() << std::endl;

    //create a context with all devices
    Context context(all_devices);
    if(!context.checkResult(CL_SUCCESS)) {
        std::cout << "Could not create context" << std::endl;
        return -1;
    }

    //create a queue on the default device
    CommandQueue queue(context, default_device);
    if(!queue.checkResult(CL_SUCCESS)) {
        std::cout << "Could not create command queue" << std::endl;
        return -1;
    }

    //allocate a buffer for A_h
    Buffer aBuffer(context, CL_MEM_READ_ONLY, SIZE * sizeof(int));
    if(!aBuffer.checkResult(CL_SUCCESS)) {
        std::cout << "Could not create A_h buffer" << std::endl;
        return -1;
    }

    //allocate a buffer for B_h
    Buffer bBuffer(context, CL_MEM_READ_ONLY, SIZE * sizeof(int));
    if(!bBuffer.checkResult(CL_SUCCESS)) {
        std::cout << "Could not create B_h buffer" << std::endl;
        return -1;
    }

    //allocate a buffer for C_h
    Buffer cBuffer(context, CL_MEM_WRITE_ONLY, THREADS * sizeof(int));
    if(!cBuffer.checkResult(CL_SUCCESS)) {
        std::cout << "Could not create C_h buffer" << std::endl;
        return -1;
    }

    queue.writeBuffer(aBuffer, true, 0, A_h);
    if(!queue.checkResult(CL_SUCCESS)) {
        std::cout << "Could not write to aBuffer" << std::endl;
        return -1;
    }

    queue.writeBuffer(bBuffer, true, 0, B_h);
    if(!queue.checkResult(CL_SUCCESS)) {
        std::cout << "Could not write to bBuffer" << std::endl;
        return -1;
    }

    //create a program to run on the device
    Program program(context, "kernels/dot.cl");
    if(!program.checkResult(CL_SUCCESS)) {
        std::cout << "Could not create program" << std::endl;
        return -1;
    }

    //try to build the program
    program.build();
    if(!program.checkResult(CL_SUCCESS)) {
        std::cout << "Could not build program" << std::endl;
        std::cout << "Error: " << program.getInfo<CL_PROGRAM_BUILD_LOG, char>(default_device) << std::endl;
        return -1;
    }

    //register the dot kernel to the program
    program.addKernel("dot");
    if(!program.checkResult(CL_SUCCESS)) {
        std::cout << "Could not add dot kernel" << std::endl;
        return -1;
    }

    //add the arguments to the dot kernel
    std::shared_ptr<Kernel> dot = program.getKernel("dot");
    dot->setArg<uint>(0, &len);
    if(!dot->checkResult(CL_SUCCESS)) {
        std::cout << "Could not set len argument" << std::endl;
        return -1;
    }

    dot->setArg(1, aBuffer);
    if(!dot->checkResult(CL_SUCCESS)) {
        std::cout << "Could not set a argument" << std::endl;
        return -1;
    }

    dot->setArg(2, bBuffer);
    if(!dot->checkResult(CL_SUCCESS)) {
        std::cout << "Could not set b argument" << std::endl;
        return -1;
    }

    dot->setArg(3, cBuffer);
    if(!dot->checkResult(CL_SUCCESS)) {
        std::cout << "Could not set c argument" << std::endl;
        return -1;
    }

    //run the kernel
    queue.runKernel<THREADS, 1>(dot, 1);
    if(!queue.checkResult(CL_SUCCESS)) {
        std::cout << "Could not run dot kernel" << std::endl;
        return -1;
    }

    //copy the data back to the host
    queue.readBuffer(cBuffer, true, 0, C_h);
    if(!queue.checkResult(CL_SUCCESS)) {
        std::cout << "Could not read C_h buffer" << std::endl;
        return -1;
    }

    dot_result_d = reduce(THREADS, C_h);
    std::cout << "Dot result on GPU: " << dot_result_d << std::endl;

    return 0;
}