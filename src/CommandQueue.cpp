#include "CommandQueue.h"
#include "Context.h"
#include "Device.h"
#include "Buffer.h"

CommandQueue::CommandQueue(Context& context, Device& device) {
    logInfo("Calling CommandQueue::CommandQueue");

    logInfo("Calling clCreateCommandQueueWithProperties");
    queue = clCreateCommandQueueWithProperties(context.getId(), device.getId(), nullptr, &result);
    logInfo("clCreateCommandQueueWithProperties called");
    
    //check result of queue creation
    if(result != CL_SUCCESS) {
        int icontext = CL_INVALID_CONTEXT;
        int idevice = CL_INVALID_DEVICE;
        int ivalue = CL_INVALID_VALUE;
        int iproperties = CL_INVALID_QUEUE_PROPERTIES;
        int oresources = CL_OUT_OF_RESOURCES;
        int omemory = CL_OUT_OF_HOST_MEMORY;
        logError("clCreateCommandQueueWithProperties()");
        return;
    }

    logInfo("CommandQueue::CommandQueue called");
}

CommandQueue::~CommandQueue() {
    logInfo("Calling clReleaseCommandQueue");
    clReleaseCommandQueue(queue);
    logInfo("clReleaseCommandQueue called");
}

void CommandQueue::writeBuffer(Buffer& buffer, bool blocking, size_t offset, const void *ptr) {
    logInfo("Calling ComandQueue::writeBuffer");

    //enqueue a write buffer command to the queue
    logInfo("Calling clEnqueueWriteBuffer");
    result = clEnqueueWriteBuffer(queue, buffer.getId(), blocking, offset, buffer.getSize(), ptr, 0, nullptr, nullptr);
    logInfo("clEnqueueWriteBuffer called");

    //check for errors
    if(result != CL_SUCCESS) {
        logError("clEnqueueWriteBuffer()");
        return;
    }

    logInfo("CommandQueue::writeBuffer called");
}

void CommandQueue::readBuffer(Buffer& buffer, bool blocking, size_t offset, void *ptr) {
    logInfo("Calling CommandQueue::readBuffer");

    //enqueue a read buffer command to the queue
    logInfo("Calling clEnqueueReadBuffer");
    result = clEnqueueReadBuffer(queue, buffer.getId(), blocking, offset, buffer.getSize(), ptr, 0, nullptr, nullptr);
    logInfo("clEnqueueReadBuffer called");

    //check for errors
    if(result != CL_SUCCESS) {
        logError("clEnqueueReadBuffer()");
        return;
    }

    logInfo("CommandQueue::readBuffer called");
}

void CommandQueue::copyBuffer(Buffer& src, Buffer& dest, size_t src_offset, size_t dest_offset) {
    logInfo("Calling CommandQueue::copyBuffer");

    //enqueue a copy buffer command to the queue
    logInfo("Calling clEnqueueCopyBuffer");
    result = clEnqueueCopyBuffer(queue, src.getId(), dest.getId(), src_offset, dest_offset, src.getSize(), 0, nullptr, nullptr);
    logInfo("clEnqueueCopyBuffer called");

    //check for errors
    if(result != CL_SUCCESS) {
        logError("clEnqueueCopyBuffer()");
        return;
    }

    logInfo("CommandQueue::copyBuffer called");
}