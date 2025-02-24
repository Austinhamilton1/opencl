#include "CommandQueue.h"
#include "Context.h"
#include "Device.h"
#include "Buffer.h"

cl::CommandQueue::CommandQueue(cl::Context& context, Device& device) {
    logInfo("Calling CommandQueue::CommandQueue");

    logInfo("Calling clCreateCommandQueueWithProperties");
    queue = clCreateCommandQueueWithProperties(context.getId(), device.getId(), nullptr, &result);
    logInfo("clCreateCommandQueueWithProperties called");
    
    //check result of queue creation
    if(result != CL_SUCCESS) {
        logError("clCreateCommandQueueWithProperties()");
        return;
    }

    logInfo("CommandQueue::CommandQueue called");
}

cl::CommandQueue::~CommandQueue() {
    logInfo("Calling clReleaseCommandQueue");
    if(queue != nullptr)
        clReleaseCommandQueue(queue);
    logInfo("clReleaseCommandQueue called");
}

void cl::CommandQueue::writeBuffer(cl::Buffer& buffer, bool blocking, size_t offset, const void *ptr) {
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

void cl::CommandQueue::readBuffer(cl::Buffer& buffer, bool blocking, size_t offset, void *ptr) {
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

void cl::CommandQueue::copyBuffer(cl::Buffer& src, cl::Buffer& dest, size_t src_offset, size_t dest_offset) {
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

void cl::CommandQueue::writeBufferRect(cl::Buffer& buffer, bool blocking, size_t offset_x, size_t offset_y, size_t offset_z, size_t width_bytes, size_t height_count, size_t depth_count, const void *data) {
    //set up data for the call
    std::vector<size_t> origin;
    std::vector<size_t> region;

    //offset data
    origin.push_back(offset_x);
    origin.push_back(offset_y);
    origin.push_back(offset_z);

    //size data
    region.push_back(width_bytes);
    region.push_back(height_count);
    region.push_back(depth_count);
    
    logInfo("Calling CommandQueue::writeBufferRect");

    //enqueue a write buffer rect command to the queue
    logInfo("Calling clEnqueueWriteBufferRect");
    result = clEnqueueWriteBufferRect(queue, buffer.getId(), blocking, origin.data(), origin.data(), region.data(), 0, 0, 0, 0, data, 0, nullptr, nullptr);
    logInfo("clEnqueueWriteBufferRect called");

    if(result != CL_SUCCESS) {
        logError("clEnqueueWriteBufferRect()");
        return;
    }

    logInfo("CommandQueue::writeBufferRect called");
}

void cl::CommandQueue::readBufferRect(cl::Buffer& buffer, bool blocking, size_t offset_x, size_t offset_y, size_t offset_z, size_t width_bytes, size_t height_count, size_t depth_count, void *data) {
    //set up data for the call
    std::vector<size_t> origin;
    std::vector<size_t> region;

    //offset data
    origin.push_back(offset_x);
    origin.push_back(offset_y);
    origin.push_back(offset_z);

    //size data
    region.push_back(width_bytes);
    region.push_back(height_count);
    region.push_back(depth_count);

    logInfo("Calling CommandQueue::readBufferRect");

    //enqueue a write buffer rect command to the queue
    logInfo("Calling clEnqueueReadBufferRect");
    result = clEnqueueReadBufferRect(queue, buffer.getId(), blocking, origin.data(), origin.data(), region.data(), 0, 0, 0, 0, data, 0, nullptr, nullptr);
    logInfo("clEnqueueReadBufferRect called");

    if(result != CL_SUCCESS) {
        logError("clEnqueueReadBufferRect()");
        return;
    }

    logInfo("CommandQueue::readBufferRect called");
}

void cl::CommandQueue::copyBufferRect(cl::Buffer& src, cl::Buffer& dest, size_t offset_x, size_t offset_y, size_t offset_z, size_t width_bytes, size_t height_count, size_t depth_count) {
    //set up data for the call
    std::vector<size_t> origin;
    std::vector<size_t> region;

    //offset data
    origin.push_back(offset_x);
    origin.push_back(offset_y);
    origin.push_back(offset_z);

    //size data
    region.push_back(width_bytes);
    region.push_back(height_count);
    region.push_back(depth_count);

    logInfo("Calling CommandQueue::copyBufferRect");

    //enqueue a write buffer rect command to the queue
    logInfo("Calling clEnqueueCopyBufferRect");
    result = clEnqueueCopyBufferRect(queue, src.getId(), dest.getId(), origin.data(), origin.data(), region.data(), 0, 0, 0, 0, 0, nullptr, nullptr);
    logInfo("clEnqueueCopyBufferRect called");

    if(result != CL_SUCCESS) {
        logError("clEnqueueCopyBufferRect()");
        return;
    }

    logInfo("CommandQueue::copyBufferRect called");
}

void cl::CommandQueue::runKernel(std::shared_ptr<cl::Kernel> kernel, size_t global_x, size_t local_x) {
    logInfo("Calling CommandQueue::runKernel");

    std::vector<size_t> global_sizes;
    std::vector<size_t> local_sizes;

    global_sizes.push_back(global_x);
    local_sizes.push_back(local_x);

    logInfo("Calling clEnqueueNDRangeKernel");
    result = clEnqueueNDRangeKernel(queue, kernel->getId(), 1, nullptr, global_sizes.data(), local_sizes.data(), 0, nullptr, nullptr);
    logInfo("clEnqueueNDRangeKernel called");

    if(result != CL_SUCCESS) {
        logError("clEnqueueNDRangeKernel()");
        return;
    }

    logInfo("CommandQueue::runKernel");
}

void cl::CommandQueue::runKernel(std::shared_ptr<cl::Kernel> kernel, size_t global_x, size_t global_y, size_t local_x, size_t local_y) {
    logInfo("Calling CommandQueue::runKernel");

    std::vector<size_t> global_sizes;
    std::vector<size_t> local_sizes;

    global_sizes.push_back(global_x);
    global_sizes.push_back(global_y);
    local_sizes.push_back(local_x);
    local_sizes.push_back(local_y);

    logInfo("Calling clEnqueueNDRangeKernel");
    result = clEnqueueNDRangeKernel(queue, kernel->getId(), 2, nullptr, global_sizes.data(), local_sizes.data(), 0, nullptr, nullptr);
    logInfo("clEnqueueNDRangeKernel called");

    if(result != CL_SUCCESS) {
        logError("clEnqueueNDRangeKernel()");
        return;
    }

    logInfo("CommandQueue::runKernel");
}

void cl::CommandQueue::runKernel(std::shared_ptr<cl::Kernel> kernel, size_t global_x, size_t global_y, size_t global_z, size_t local_x, size_t local_y, size_t local_z) {
    logInfo("Calling CommandQueue::runKernel");

    std::vector<size_t> global_sizes;
    std::vector<size_t> local_sizes;

    global_sizes.push_back(global_x);
    global_sizes.push_back(global_y);
    global_sizes.push_back(global_z);
    local_sizes.push_back(local_x);
    local_sizes.push_back(local_y);
    local_sizes.push_back(local_z);

    logInfo("Calling clEnqueueNDRangeKernel");
    result = clEnqueueNDRangeKernel(queue, kernel->getId(), 3, nullptr, global_sizes.data(), local_sizes.data(), 0, nullptr, nullptr);
    logInfo("clEnqueueNDRangeKernel called");

    if(result != CL_SUCCESS) {
        logError("clEnqueueNDRangeKernel()");
        return;
    }

    logInfo("CommandQueue::runKernel");
}