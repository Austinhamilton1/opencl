#pragma once
#include <memory>

#include <CL/cl.h>

#include "Logger.h"
#include "Buffer.h"
#include "Kernel.h"

class CommandQueue : public Logger {
private:
    cl_command_queue queue;

public:
    CommandQueue(cl_command_queue queue) : queue(queue) {};
    ~CommandQueue();

    inline cl_command_queue& getId() { return queue; };

    void writeBuffer(Buffer& buffer, bool blocking, size_t offset, const void *ptr);
    void readBuffer(Buffer& buffer, bool blocking, size_t offset, void *ptr);
    void copyBuffer(Buffer& src, Buffer& dest, size_t src_offset, size_t dest_offset);
    
    template <unsigned int x, unsigned int y, unsigned int z> void runKernel(Kernel& kernel) {
        std::vector<size_t> global_work_size;

        logInfo("Calling CommandQueue::runKernel");

        //set the global work size array
        global_work_size.push_back(x);
        global_work_size.push_back(y);
        global_work_size.push_back(z);

        logInfo("Calling clEnqueueNDRangeKernel");
        result = clEnqueueNDRangeKernel(queue, kernel.getId(), 3, nullptr, global_work_size.data(), nullptr, 0, nullptr, nullptr);
        logInfo("clEnqueueNDRangeKernel called");

        if(result != CL_SUCCESS) {
            logError("clEnqueueNDRangeKernel()");
            return;
        }

        logInfo("CommandQueue::runKernel");
    }

    template <cl_context_info S, typename T> std::shared_ptr<T[]> getInfo() {
        std::string message;
        size_t param_size; //size of the parameter
        
        logInfo("Calling CommandQueue::getInfo");

        //get the size of the parameter
        logInfo("Calling clGetCommandQueueInfo");
        result = clGetCommandQueueInfo(queue, S, 0, nullptr, &param_size);
        logInfo("clGetCommandQueueInfo called");

        if(result != CL_SUCCESS) {
            logError("clGetCommandQueueInfo()");
            return nullptr;
        }

        //allocate the shared pointer
        logInfo("Creating shared pointer");
        std::shared_ptr<T[]> value(new T[param_size/sizeof(T)], std::default_delete<T[]>());
        logInfo("Shared pointer created");

        //get queue info
        logInfo("Calling clGetCommandQueueInfo");
        result = clGetCommandQueueInfo(queue, S, param_size, value.get(), nullptr);
        logInfo("clGetCommandQueueInfo called");

        if(result != CL_SUCCESS) {
            logError("clGetCommandQueueInfo()");
            return nullptr;
        }

        logInfo("CommandQueue::getInfo called");

        return value;
    }
};