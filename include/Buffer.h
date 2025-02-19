#pragma once
#include <CL/cl.h>

#include "Logger.h"

class Context;

class Buffer : public Logger {
private:
    cl_mem buffer;
    size_t size;

public:
    Buffer(Context& context, cl_mem_flags flags, size_t size);
    ~Buffer() {};

    inline cl_mem& getId() { return buffer; };
    inline size_t getSize() { return size; };
};