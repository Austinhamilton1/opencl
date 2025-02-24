#include "Buffer.h"
#include "Context.h"

cl::Buffer::Buffer(cl::Context& context, cl_mem_flags flags, size_t size) {
    this->size = size;

    logInfo("Creating Buffer");
    
    logInfo("Calling clCreateBuffer");
    buffer = clCreateBuffer(context.getId(), flags, size, nullptr, &result);
    logInfo("clCreateBuffer called");

    if(result != CL_SUCCESS)
        logError("clCreateBuffer()");

    logInfo("Buffer created");
}

cl::Buffer::~Buffer() {
    logInfo("Calling clReleaseMemObject");
    if(buffer != nullptr)
        clReleaseMemObject(buffer);
    logInfo("clReleaseMemObject called");
}