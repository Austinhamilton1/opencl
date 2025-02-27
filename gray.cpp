#include <vector>
#include <string>
#include <iostream>
#include <fstream>
#include <sstream>
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
        std::cout << "Usage: gray [src filename] [dest filename]" << std::endl;
        return -1;
    }

    //read in the image file
    PPMImage image;
    image.fromFile(argv[1]);
    image.normalize();

    std::cout << "Read file of size " << image.width << " x " << image.height << std::endl;

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

    //allocate the buffer for the image data
    cl::Buffer imgBuffer(context, CL_MEM_READ_ONLY, image.width * image.height * 3 * sizeof(float));
    if(!imgBuffer.checkResult(CL_SUCCESS)) {
        std::cout << "Failed to create image buffer: " << imgBuffer.getResultString() << std::endl;
        return -1;
    }

    //allocate the buffer for the gray scale image
    cl::Buffer grayBuffer(context, CL_MEM_WRITE_ONLY, image.width * image.height * 3 * sizeof(float));
    if(!grayBuffer.checkResult(CL_SUCCESS)) {
        std::cout << "Failed to create gray image buffer: " << grayBuffer.getResultString() << std::endl;
        return -1;
    }

    //write the image data to the buffer
    queue.writeBuffer(imgBuffer, true, 0, image.norm.data());
    if(!queue.checkResult(CL_SUCCESS)) {
        std::cout << "Failed to write to image buffer: " << queue.getResultString() << std::endl;
        return -1;
    }

    //create the program
    cl::Program program(context, "kernels/gray.cl");
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
    program.addKernel("gray");
    if(!program.checkResult(CL_SUCCESS)) {
        std::cout << "Failed to add kernel: " << program.getResultString() << std::endl;
        return -1;
    }

    //set the arguments of the kernel
    std::shared_ptr<cl::Kernel> gray = program.getKernel("gray");

    gray->setArg(0, imgBuffer);
    if(!gray->checkResult(CL_SUCCESS)) {
        std::cout << "Failed to set image argument: " << gray->getResultString() << std::endl;
        return -1;
    }

    gray->setArg(1, grayBuffer);
    if(!gray->checkResult(CL_SUCCESS)) {
        std::cout << "Failed to set gray image argument: " << gray->getResultString() << std::endl;
        return -1;
    }

    //run the gray scale kernel
    queue.runKernel(gray, image.height, image.width, 1, 1);
    if(!queue.checkResult(CL_SUCCESS)) {
        std::cout << "Failed to run gray scale kernel: " << queue.getResultString() << std::endl;
        return -1;
    }

    //allocate an image object 
    PPMImage gray_image;
    gray_image.width = image.width;
    gray_image.height = image.height;
    gray_image.max_color_value = image.max_color_value;
    gray_image.norm.resize(gray_image.width * gray_image.height * 3);

    //run the kernel
    queue.readBuffer(grayBuffer, true, 0, gray_image.norm.data());
    if(!queue.checkResult(CL_SUCCESS)) {
        std::cout << "Failed to read sobel image buffer: " << queue.getResultString() << std::endl;
        return -1;
    }

    gray_image.loadNorm();

    gray_image.toFile(argv[2]);

    std::cout << "Image successfully written to " << argv[2] << std::endl;

    return 0;
}