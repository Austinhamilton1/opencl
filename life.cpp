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

#define BOARD_SIZE 64

void printBoard(int board[BOARD_SIZE][BOARD_SIZE]) {
    for(int i = 0; i < BOARD_SIZE; i++) {
        for(int j = 0; j < BOARD_SIZE; j++) {
            std::cout << board[i][j] << " ";
        }
        std::cout << std::endl;
    }
}

int main() {
    int board[BOARD_SIZE][BOARD_SIZE], next_board[BOARD_SIZE][BOARD_SIZE];
    std::mt19937 generator(std::time(0));
    std::uniform_int_distribution<int> distribution(0, 1);

    for(int i = 0; i < BOARD_SIZE; i++) {
        for(int j = 0; j < BOARD_SIZE; j++) {
            board[i][j] = distribution(generator);
        }
    }

    //get list of available platforms
    std::vector<cl::Platform> all_platforms = cl::Platform::allPlatforms();
    if(all_platforms.size() == 0) {
        std::cout << "No compute platforms found" << std::endl;
        return 0;
    }

    //set default platform
    cl::Platform default_platform = all_platforms[0];
    std::cout << "Using platform: " << default_platform.getInfo<CL_PLATFORM_NAME, char>() << std::endl;

    //get list of available devices
    std::vector<cl::Device> all_devices = cl::Device::allDevices(default_platform);
    if(all_devices.size() == 0) {
        std::cout << "No computing devices found" << std::endl;
        return 0;
    }

    //set default device
    cl::Device default_device = all_devices[0];
    std::cout << "Using device: " << default_device.getInfo<CL_DEVICE_NAME, char>() << std::endl;

    //create a context on the devices
    cl::Context context(all_devices);
    if(!context.checkResult(CL_SUCCESS)) {
        std::cout << "Failed to create context: " << context.getResultString() << std::endl;
        return -1;
    }

    //create a device command queue on the default device
    cl::CommandQueue queue(context, default_device);
    if(!context.checkResult(CL_SUCCESS)) {
        std::cout << "Failed to create command queue: " << queue.getResultString() << std::endl;
        return -1;
    }

    //allocate the board buffers
    cl::Buffer boardBuffer(context, CL_MEM_READ_ONLY, BOARD_SIZE * BOARD_SIZE * sizeof(int));
    cl::Buffer nextBuffer(context, CL_MEM_WRITE_ONLY, BOARD_SIZE * BOARD_SIZE * sizeof(int));

    if(!boardBuffer.checkResult(CL_SUCCESS)) {
        std::cout << "Failed to create board buffer: " << boardBuffer.getResultString() << std::endl;
        return -1;
    }

    if(!nextBuffer.checkResult(CL_SUCCESS)) {
        std::cout << "Failed to create next board buffer: " << nextBuffer.getResultString() << std::endl;
        return -1;
    }

    //write the data to the board buffer
    queue.writeBufferRect(boardBuffer, true, 0, 0, 0, BOARD_SIZE * sizeof(int), BOARD_SIZE, 1, board);
    if(!queue.checkResult(CL_SUCCESS)) {
        std::cout << "Failed to write to board buffer: " << boardBuffer.getResultString() << std::endl;
        return -1;
    }

    //create the program
    cl::Program program(context, "kernels/life.cl");
    if(!program.checkResult(CL_SUCCESS)) {
        std::cout << "Failed to create program: " << program.getResultString() << std::endl;
        return -1;
    }

    //build the program
    program.build();
    if(!program.checkResult(CL_SUCCESS)) {
        std::cout << "Failed to build program: " << program.getResultString() << std::endl;
        std::cout << "Build log: " << program.getInfo<CL_PROGRAM_BUILD_LOG, char>(default_device);
        return -1;
    }

    //add the kernel
    program.addKernel("life");
    if(!program.checkResult(CL_SUCCESS)) {
        std::cout << "Failed to add kernel: " << program.getResultString() << std::endl;
        return -1;
    }

    //set the kernel arguments
    std::shared_ptr<cl::Kernel> life = program.getKernel("life");
    
    life->setArg(0, boardBuffer);
    if(!life->checkResult(CL_SUCCESS)) {
        std::cout << "Failed to add board argument: " << life->getResultString() << std::endl;
        return -1;
    }
    life->setArg(1, nextBuffer);
    if(!life->checkResult(CL_SUCCESS)) {
        std::cout << "Failed to add next board argument: " << life->getResultString() << std::endl;
        return -1;
    }

    //run the kernel
    queue.runKernel<BOARD_SIZE, BOARD_SIZE, 1, 1>(life);
    if(!queue.checkResult(CL_SUCCESS)) {
        std::cout << "Failed to run kernel: " << queue.getResultString() << std::endl;
        return -1;
    }

    //read the data back from the next board buffer
    queue.readBufferRect(nextBuffer, true, 0, 0, 0, BOARD_SIZE * sizeof(int), BOARD_SIZE, 1, next_board);
    if(!queue.checkResult(CL_SUCCESS)) {
        std::cout << "Failed to read data back to host: " << queue.getResultString() << std::endl;
    }

    //print the two boards
    printBoard(board);

    std::cout << std::endl;

    printBoard(next_board);

    return 0;
}