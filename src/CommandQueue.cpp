#include "CommandQueue.h"

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