#pragma once
#include <vector>
#include <memory>
#include <unordered_map>
#include <string>

#include <CL/cl.h>

#include "Logger.h"
#include "Device.h"
#include "Program.h"

namespace cl {
    class Context : public Logger {
    private:
        cl_context context;
        std::vector<Device> devices;
        std::unordered_map<std::string, Program> programs;

    public:
        Context(std::vector<Device> devices);
        ~Context();

        inline cl_context& getId() { return context; };
        inline std::vector<Device>& getDevices() { return devices; };

        template <cl_context_info S, typename T> std::shared_ptr<T[]> getInfo() {
            std::string message;
            size_t param_size; //size of the parameter
            
            logInfo("Calling Context::getInfo");

            //get the size of the parameter
            logInfo("Calling clGetContextInfo");
            result = clGetContextInfo(context, S, 0, nullptr, &param_size);
            logInfo("clGetContextInfo called");

            if(result != CL_SUCCESS) {
                logError("clGetContextInfo()");
                return nullptr;
            }

            //allocate the shared pointer
            logInfo("Creating shared pointer");
            std::shared_ptr<T[]> value(new T[param_size/sizeof(T)], std::default_delete<T[]>());
            logInfo("Shared pointer created");

            //get context info
            logInfo("Calling clGetContextInfo");
            result = clGetContextInfo(context, S, param_size, value.get(), nullptr);
            logInfo("clGetContextInfo called");

            if(result != CL_SUCCESS) {
                logError("clGetContextInfo()");
                return nullptr;
            }

            logInfo("Context::getInfo called");

            return value;
        }
    };
}