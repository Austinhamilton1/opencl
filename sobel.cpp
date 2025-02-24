#include <vector>
#include <string>
#include <iostream>
#include <fstream>
#include <sstream>
#include <memory>

#include <stdio.h>

#include "Platform.h"
#include "Device.h"
#include "Context.h"
#include "CommandQueue.h"
#include "Buffer.h"
#include "Program.h"
#include "Kernel.h"

#define WIDTH 1500
#define HEIGHT 1000


// Function to read a PPM file and return a 3D RGB array
std::vector<std::vector<std::vector<float>>> readPPM(const char *filename) {
    std::ifstream file(filename, std::ios::binary);
    if (!file) {
        std::cerr << "Error: Unable to open file " << filename << std::endl;
        exit(1);
    }

    std::string format;
    int width, height, maxVal;
    
    // Read PPM header
    file >> format >> width >> height >> maxVal;
    file.ignore();  // Ignore the newline character after maxVal

    if (format != "P3" && format != "P6") {
        std::cerr << "Error: Unsupported PPM format (only P3 and P6 are supported)." << std::endl;
        exit(1);
    }

    // Allocate 3D array: height x width x 3 (RGB)
    std::vector<std::vector<std::vector<float>>> image(height, std::vector<std::vector<float>>(width, std::vector<float>(3)));

    if (format == "P3") { // ASCII format
        for (int i = 0; i < height; ++i) {
            for (int j = 0; j < width; ++j) {
                int r, g, b;
                file >> r >> g >> b;
                image[i][j][0] = r / static_cast<float>(maxVal);
                image[i][j][1] = g / static_cast<float>(maxVal);
                image[i][j][2] = b / static_cast<float>(maxVal);
            }
        }
    } else if (format == "P6") { // Binary format
        file.get();  // Skip the single whitespace character after maxVal
        for (int i = 0; i < height; ++i) {
            for (int j = 0; j < width; ++j) {
                unsigned char rgb[3];
                file.read(reinterpret_cast<char*>(rgb), 3);
                image[i][j][0] = rgb[0] / static_cast<float>(maxVal);
                image[i][j][1] = rgb[1] / static_cast<float>(maxVal);
                image[i][j][2] = rgb[2] / static_cast<float>(maxVal);
            }
        }
    }

    file.close();
    return image;
}

// Function to write a 3D float vector (normalized RGB values) to a binary PPM file
void writePPM(const char* filename, const std::vector<std::vector<float>>& image) {
    if (image.empty() || image[0].empty()) {
        std::cerr << "Error: Invalid image dimensions!" << std::endl;
        return;
    }

    int height = image.size();
    int width = image[0].size();
    int maxVal = 255;  // Standard 8-bit PPM

    std::ofstream file(filename, std::ios::binary);
    if (!file) {
        std::cerr << "Error: Unable to open file for writing!" << std::endl;
        return;
    }

    // Write PGM header
    file << "P5\n" << width << " " << height << "\n" << maxVal << "\n";

    // Write binary pixel data
    for (int i = 0; i < height; ++i) {
        for (int j = 0; j < width; ++j) {
            unsigned char pixel = static_cast<unsigned char>(image[i][j] * maxVal);
            file.write(reinterpret_cast<char*>(&pixel), 1);
        }
    }

    file.close();
    std::cout << "PPM image successfully written to: " << filename << std::endl;
}

int main(int argc, char *argv[]) {
    if(argc < 3) {
        std::cout << "Usage: sobel [src filename] [dest filename]" << std::endl;
        return -1;
    }

    //read in the image file
    std::vector<std::vector<std::vector<float>>> rgb = readPPM(argv[1]);
    const uint height = rgb.size();
    const uint width = rgb[0].size();

    std::cout << "Read file of size " << rgb.size() << " x " << rgb[0].size() << std::endl;

    //allocate memory for buffer
    std::vector<float> img_data;
    std::vector<float> gray_data;
    for(int i = 0; i < height; i++) {
        for(int j = 0; j < width; j++) {
            img_data.push_back(rgb[i][j][0]);
            img_data.push_back(rgb[i][j][1]);
            img_data.push_back(rgb[i][j][2]);
            gray_data.push_back(0.0f);
        }
    }
    std::vector<float> sobel_data((width - 2) * (height - 2));

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
    cl::Buffer imgBuffer(context, CL_MEM_READ_ONLY, width * height * 3 * sizeof(float));
    if(!imgBuffer.checkResult(CL_SUCCESS)) {
        std::cout << "Failed to create image buffer: " << imgBuffer.getResultString() << std::endl;
        return -1;
    }

    //allocate the buffer for the gray scale image
    cl::Buffer grayBuffer(context, CL_MEM_READ_WRITE, width * height * sizeof(float));
    if(!grayBuffer.checkResult(CL_SUCCESS)) {
        std::cout << "Failed to create gray image buffer: " << grayBuffer.getResultString() << std::endl;
        return -1;
    }

    //allocate the buffer for the sobel image 
    cl::Buffer sobelBuffer(context, CL_MEM_WRITE_ONLY, (width - 2) * (height - 2) * sizeof(float));
    if(!sobelBuffer.checkResult(CL_SUCCESS)) {
        std::cout << "Failed to create sobel image buffer: " << sobelBuffer.getResultString() << std::endl;
        return -1;
    }

    //write the image data to the buffer
    queue.writeBuffer(imgBuffer, true, 0, img_data.data());
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

    //add the grayscale kernel
    program.addKernel("gray");
    if(!program.checkResult(CL_SUCCESS)) {
        std::cout << "Failed to add kernel: " << program.getResultString() << std::endl;
        return -1;
    }

    //add the sobel image edge detection kernel
    program.addKernel("sobel");
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

    //set the arguments of the kernel
    std::shared_ptr<cl::Kernel> sobel = program.getKernel("sobel");

    sobel->setArg(0, grayBuffer);
    if(!sobel->checkResult(CL_SUCCESS)) {
        std::cout << "Failed to set gray image argument: " << sobel->getResultString() << std::endl;
        return -1;
    }

    sobel->setArg(1, sobelBuffer);
    if(!sobel->checkResult(CL_SUCCESS)) {
        std::cout << "Failed to set sobel image argument: " << sobel->getResultString() << std::endl;
        return -1;
    }

    //run the gray scale kernel
    queue.runKernel<HEIGHT, WIDTH, 1, 1>(gray);
    if(!queue.checkResult(CL_SUCCESS)) {
        std::cout << "Failed to run gray scale kernel: " << queue.getResultString() << std::endl;
        return -1;
    }

    //run the sobel kernel
    queue.runKernel<HEIGHT, WIDTH, 1, 1>(sobel);
    if(!queue.checkResult(CL_SUCCESS)) {
        std::cout << "Failed to run sobel kernel: " << queue.getResultString() << std::endl;
        return -1;
    }

    //run the kernel
    queue.readBuffer(sobelBuffer, true, 0, sobel_data.data());
    if(!queue.checkResult(CL_SUCCESS)) {
        std::cout << "Failed to read sobel image buffer: " << queue.getResultString() << std::endl;
        return -1;
    }

    //write the gray scale image to a file
    std::vector<std::vector<float>> sobelVec(height - 2, std::vector<float>(width - 2));
    for(uint i = 0; i < height - 2; i++) {
        for(uint j = 0; j < width - 2; j++) {
            sobelVec[i][j] = sobel_data[i * width + j];
        }
    }

    writePPM(argv[2], sobelVec);

    return 0;
}