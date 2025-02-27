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
    if(argc < 4) {
        std::cout << "Usage: gauss [sigma] [src filename] [dest filename]" << std::endl;
        return -1;
    }

    float sigma = std::stof(argv[1]);

    //read in the image file
    PPMImage image;
    image.fromFile(argv[2]);
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
    cl::Buffer outBuffer(context, CL_MEM_WRITE_ONLY, image.width * image.height * 3 * sizeof(float));
    if(!outBuffer.checkResult(CL_SUCCESS)) {
        std::cout << "Failed to create gray image buffer: " << outBuffer.getResultString() << std::endl;
        return -1;
    }

    //write the image data to the buffer
    queue.writeBuffer(imgBuffer, true, 0, image.norm.data());
    if(!queue.checkResult(CL_SUCCESS)) {
        std::cout << "Failed to write to image buffer: " << queue.getResultString() << std::endl;
        return -1;
    }

    //create the program
    cl::Program program(context, "kernels/gauss.cl");
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
    program.addKernel("gauss");
    if(!program.checkResult(CL_SUCCESS)) {
        std::cout << "Failed to add kernel: " << program.getResultString() << std::endl;
        return -1;
    }

    //set the arguments of the kernel
    std::shared_ptr<cl::Kernel> gauss = program.getKernel("gauss");

    gauss->setArg<float>(0, &sigma);
    if(!gauss->checkResult(CL_SUCCESS)) {
        std::cout << "Failed to set image argument: " << gauss->getResultString() << std::endl;
        return -1;
    }

    gauss->setArg(1, imgBuffer);
    if(!gauss->checkResult(CL_SUCCESS)) {
        std::cout << "Failed to set image argument: " << gauss->getResultString() << std::endl;
        return -1;
    }

    gauss->setArg(2, outBuffer);
    if(!gauss->checkResult(CL_SUCCESS)) {
        std::cout << "Failed to set gray image argument: " << gauss->getResultString() << std::endl;
        return -1;
    }

    //run the gauss kernel
    queue.runKernel(gauss, image.height, image.width, 1, 1);
    if(!queue.checkResult(CL_SUCCESS)) {
        std::cout << "Failed to run gauss kernel: " << queue.getResultString() << std::endl;
        return -1;
    }

    PPMImage out_image;
    out_image.width = image.width;
    out_image.height = image.height;
    out_image.max_color_value = 255;
    out_image.norm.resize(out_image.width * out_image.height * 3);

    //run the kernel
    queue.readBuffer(outBuffer, true, 0, out_image.norm.data());
    if(!queue.checkResult(CL_SUCCESS)) {
        std::cout << "Failed to read gauss image buffer: " << queue.getResultString() << std::endl;
        return -1;
    }

    out_image.loadNorm();
    out_image.toFile(argv[3]);

    std::cout << "Image successfully written to " << argv[3] << std::endl;

    return 0;
}