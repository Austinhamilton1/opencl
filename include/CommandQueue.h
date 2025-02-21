#pragma once
#include <memory>
#include <vector>

#include <CL/cl.h>

#include "Logger.h"
#include "Kernel.h"

class Buffer;
class Context;
class Device;

namespace cl {
    class CommandQueue : public Logger {
    private:
        cl_command_queue queue;

    public:
        CommandQueue(Context& context, Device& device);
        ~CommandQueue();

        inline cl_command_queue& getId() { return queue; };

        void writeBuffer(Buffer& buffer, bool blocking, size_t offset, const void *ptr);
        void readBuffer(Buffer& buffer, bool blocking, size_t offset, void *ptr);
        void copyBuffer(Buffer& src, Buffer& dest, size_t src_offset, size_t dest_offset);

        void writeBufferRect(Buffer& buffer, bool blocking, size_t offset_x, size_t offset_y, size_t offset_z, size_t width_bytes, size_t height_count, size_t depth_count, const void *data);
        void readBufferRect(Buffer& buffer, bool blocking, size_t offset_x, size_t offset_y, size_t offset_z, size_t width_bytes, size_t height_count, size_t depth_count, void *data);
        
        template <size_t global, size_t local> void runKernel(std::shared_ptr<Kernel> kernel) {
            logInfo("Calling CommandQueue::runKernel");

            std::vector<size_t> global_sizes;
            std::vector<size_t> local_sizes;

            global_sizes.push_back(global);
            local_sizes.push_back(local);

            logInfo("Calling clEnqueueNDRangeKernel");
            result = clEnqueueNDRangeKernel(queue, kernel->getId(), 1, nullptr, global_sizes.data(), local_sizes.data(), 0, nullptr, nullptr);
            logInfo("clEnqueueNDRangeKernel called");

            if(result != CL_SUCCESS) {
                logError("clEnqueueNDRangeKernel()");
                return;
            }

            logInfo("CommandQueue::runKernel");
        };

        template <size_t global_x, size_t global_y, size_t local_x, size_t local_y> void runKernel(std::shared_ptr<Kernel> kernel) {
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
        };

        template <size_t global_x, size_t global_y, size_t global_z, size_t local_x, size_t local_y, size_t local_z> void runKernel(std::shared_ptr<Kernel> kernel) {
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
        };

        template <cl_command_queue_info S, typename T> std::shared_ptr<T[]> getInfo() {
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
        };
    };
}