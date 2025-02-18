#include "Kernel.h"

Kernel::~Kernel() {
    clReleaseKernel(kernel);
}

void Kernel::setArg(unsigned int index, Buffer& buffer) {
    logInfo("Calling Kernel::setArg");

    logInfo("Calling clSetKernelArg");
    result = clSetKernelArg(kernel, index, sizeof(cl_mem), &buffer.getId());
    logInfo("clSetKernelArg called");

    if(result != CL_SUCCESS) {
        logError("clSetKernelArg()");
        return;
    }

    logInfo("Kernel::setArg called");
}