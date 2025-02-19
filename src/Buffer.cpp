#include "Buffer.h"
#include "Context.h"

Buffer::Buffer(Context& context, cl_mem_flags flags, size_t size) {
    this->size = size;

    logInfo("Creating Buffer");
    
    logInfo("Calling clCreateBuffer");
    buffer = clCreateBuffer(context.getId(), flags, size, nullptr, &result);
    logInfo("clCreateBuffer called");

    if(result != CL_SUCCESS)
        logError("clCreateBuffer()");

    logInfo("Buffer created");
}