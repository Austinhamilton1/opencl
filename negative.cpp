#include <vector>
#include <string>
#include <iostream>
#include <memory>

#include "Utils.h"
#include "Platform.h"
#include "Device.h"
#include "Context.h"
#include "CommandQueue.h"
#include "Buffer.h"
#include "Program.h"
#include "Kernel.h"

int main(int argc, char *argv[]) {
    if(argc < 3) {
        std::cerr << "Usage: negative [original image] [negative image]" << std::endl;
        return -1;
    }

    //read the input image
    PPMImage input;
    input.fromFile(argv[1]);
    input.normalize();

    //look for available compute platforms
    std::vector<cl::Platform> all_platforms = cl::Platform::allPlatforms();
    if(all_platforms.size() == 0) {
        std::cout << "No available platforms" << std::endl;
        return 0;
    }

    //retrieve default compute platform
    cl::Platform default_platform = all_platforms[0];
    std::cout << "Using platform: " << default_platform.getInfo<CL_PLATFORM_NAME, char>() << std::endl;

    //look for available compute devices
    std::vector<cl::Device> all_devices = cl::Device::allDevices(default_platform);
    if(all_devices.size() == 0) {
        std::cout << "No available devices" << std::endl;
        return 0;
    }

    //retrieve default compute device
    cl::Device default_device = all_devices[0];
    std::cout << "Using device: " << default_device.getInfo<CL_DEVICE_NAME, char>() << std::endl;

    //create the context
    cl::Context context(all_devices);
    if(!context.checkResult(CL_SUCCESS)) {
        std::cout << "Failed to create context: " << context.getResultString() << std::endl;
        return -1;
    }

    //create the command queue
    cl::CommandQueue queue(context, default_device);
    if(!queue.checkResult(CL_SUCCESS)) {
        std::cout << "Failed to create command queue: " << queue.getResultString() << std::endl;
        return -1;
    }

    //create the input buffer
    cl::Buffer inputBuffer(context, CL_MEM_READ_ONLY, input.width * input.height * 3 * sizeof(float));
    if(!inputBuffer.checkResult(CL_SUCCESS)) {
        std::cout << "Failed to create input buffer: " << inputBuffer.getResultString() << std::endl;
        return -1;
    }

    //create the output buffer
    cl::Buffer outputBuffer(context, CL_MEM_WRITE_ONLY, input.width * input.height * 3 * sizeof(float));
    if(!outputBuffer.checkResult(CL_SUCCESS)) {
        std::cout << "Failed to create input buffer: " << outputBuffer.getResultString() << std::endl;
        return -1;
    }

    //write the input data to the buffer
    queue.writeBuffer(inputBuffer, true, 0, input.norm.data());
    if(!queue.checkResult(CL_SUCCESS)) {
        std::cout << "Failed to write to input buffer: " << queue.getResultString() << std::endl;
        return -1;
    }

    //create the program
    cl::Program program(context, "kernels/negative.cl");
    if(!program.checkResult(CL_SUCCESS)) {
        std::cout << "Failed to create program: " << program.getResultString() << std::endl;
        return -1;
    }

    //build the program
    program.build();
    if(!program.checkResult(CL_SUCCESS)) {
        std::cout << "Failed to build program: " << program.getResultString() << std::endl;
        std::cout << "Build log: " << program.getInfo<CL_PROGRAM_BUILD_LOG, char>(default_device) << std::endl;
        return -1;
    }

    //add the grayscale kernel
    program.addKernel("negative");
    if(!program.checkResult(CL_SUCCESS)) {
        std::cout << "Failed to add kernel: " << program.getResultString() << std::endl;
        return -1;
    }

    //set the arguments of the kernel
    std::shared_ptr<cl::Kernel> negative = program.getKernel("negative");

    negative->setArg(0, inputBuffer);
    if(!negative->checkResult(CL_SUCCESS)) {
        std::cout << "Failed to set input image argument: " << negative->getResultString() << std::endl;
        return -1;
    }

    negative->setArg(1, outputBuffer);
    if(!negative->checkResult(CL_SUCCESS)) {
        std::cout << "Failed to set output argument: " << negative->getResultString() << std::endl;
        return -1;
    }

    //run the gauss kernel
    queue.runKernel(negative, input.height, input.width, 1, 1);
    if(!queue.checkResult(CL_SUCCESS)) {
        std::cout << "Failed to run color kernel: " << queue.getResultString() << std::endl;
        return -1;
    }

    PPMImage output;
    output.width = input.width;
    output.height = input.height;
    output.max_color_value = 255;
    output.norm.resize(output.width * output.height * 3);

    //run the kernel
    queue.readBuffer(outputBuffer, true, 0, output.norm.data());
    if(!queue.checkResult(CL_SUCCESS)) {
        std::cout << "Failed to read color image buffer: " << queue.getResultString() << std::endl;
        return -1;
    }

    output.loadNorm();
    output.toFile(argv[2]);

    std::cout << "Image successfully written to " << argv[2] << std::endl;

    return 0;
}