#include "Kernel.h"
#include "Buffer.h"

cl::Kernel::~Kernel() {
    clReleaseKernel(kernel);
}

void cl::Kernel::setArg(unsigned int index, cl::Buffer& buffer) {
    logInfo("Calling Kernel::setArg");
    size_t arg_size = sizeof(cl_mem);

    logInfo("Calling clSetKernelArg");
    result = clSetKernelArg(kernel, index, arg_size, &buffer.getId());
    logInfo("clSetKernelArg called");

    if(result != CL_SUCCESS) {
        logError("clSetKernelArg()");
        return;
    }

    logInfo("Kernel::setArg called");
}