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
        std::cout << "Usage: sobel [src filename] [dest filename]" << std::endl;
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

    //allocate the buffer for the sobel image 
    cl::Buffer sobelBuffer(context, CL_MEM_WRITE_ONLY, image.width * image.height * 3 * sizeof(float));
    if(!sobelBuffer.checkResult(CL_SUCCESS)) {
        std::cout << "Failed to create sobel image buffer: " << sobelBuffer.getResultString() << std::endl;
        return -1;
    }

    //write the image data to the buffer
    queue.writeBuffer(imgBuffer, true, 0, image.norm.data());
    if(!queue.checkResult(CL_SUCCESS)) {
        std::cout << "Failed to write to image buffer: " << queue.getResultString() << std::endl;
        return -1;
    }

    //create the program
    cl::Program program(context, "kernels/sobel.cl");
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

    //add the sobel image edge detection kernel
    program.addKernel("sobel");
    if(!program.checkResult(CL_SUCCESS)) {
        std::cout << "Failed to add kernel: " << program.getResultString() << std::endl;
        return -1;
    }

    //set the arguments of the kernel
    std::shared_ptr<cl::Kernel> sobel = program.getKernel("sobel");

    sobel->setArg(0, imgBuffer);
    if(!sobel->checkResult(CL_SUCCESS)) {
        std::cout << "Failed to set gray image argument: " << sobel->getResultString() << std::endl;
        return -1;
    }

    sobel->setArg(1, sobelBuffer);
    if(!sobel->checkResult(CL_SUCCESS)) {
        std::cout << "Failed to set sobel image argument: " << sobel->getResultString() << std::endl;
        return -1;
    }

    //run the sobel kernel
    queue.runKernel(sobel, image.height, image.width, 1, 1);
    if(!queue.checkResult(CL_SUCCESS)) {
        std::cout << "Failed to run sobel kernel: " << queue.getResultString() << std::endl;
        return -1;
    }

    //allocate an image object 
    PPMImage sobel_image;
    sobel_image.width = image.width;
    sobel_image.height = image.height;
    sobel_image.max_color_value = image.max_color_value;
    sobel_image.norm.resize(sobel_image.width * sobel_image.height * 3);

    //run the kernel
    queue.readBuffer(sobelBuffer, true, 0, sobel_image.norm.data());
    if(!queue.checkResult(CL_SUCCESS)) {
        std::cout << "Failed to read sobel image buffer: " << queue.getResultString() << std::endl;
        return -1;
    }

    sobel_image.loadNorm();

    sobel_image.toFile(argv[2]);

    std::cout << "Image successfully written to " << argv[2] << std::endl;

    return 0;
}