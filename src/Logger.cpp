#include <ctime>

#include <CL/cl.h>

#include "Logger.h"

std::string cl::Logger::getMessage(unsigned int level) {
    std::string message;

    //get info level messages
    if((level | LOG_LEVEL_INFO) > 0) {
        message += "INFO\n";
        message += getInfo() + "\n";
    }

    //get debug level messages
    if((level | LOG_LEVEL_DEBUG) > 0) {
        message += "DEBUG\n";
        message += getDebug() + "\n";
    }

    //get error level messages
    if((level | LOG_LEVEL_ERROR) > 0) {
        message += "ERROR\n";
        message += getError() + "\n";
    }

    return message;
}

void cl::Logger::logInfo(std::string message) {
    //add the time and message to info
    auto now = std::chrono::high_resolution_clock::now();
    auto duration  = std::chrono::duration_cast<std::chrono::microseconds>(now - init_time);

    //create the line
    std::string infoLine = "INFO: (";
    infoLine += std::to_string(duration.count());
    infoLine += ") - " + message;
    infoLine += "\n";
    info += infoLine;
}

void cl::Logger::logDebug(std::string message) {
    //add the time and message to debug
    auto now = std::chrono::high_resolution_clock::now();
    auto duration  = std::chrono::duration_cast<std::chrono::microseconds>(now - init_time);

    //create the line
    std::string debugLine = "DEBUG: (";
    debugLine += std::to_string(duration.count());
    debugLine += ") - " + message;
    debugLine += "\n";
    debug += debugLine;
}

void cl::Logger::logError(std::string message) {
    //add the time and message to error
    auto now = std::chrono::high_resolution_clock::now();
    auto duration  = std::chrono::duration_cast<std::chrono::microseconds>(now - init_time);

    //create the line
    std::string errorLine = "ERROR: (";
    errorLine += std::to_string(duration.count());
    errorLine += ") - " + message;
    errorLine += "\n";
    error += errorLine;
}

std::string cl::Logger::getResultString() {
    std::string result_str;

    //get the string representation of the logger's result
    switch(result) {
		case CL_SUCCESS:
			result_str = "CL_SUCCESS";
			break;
		case CL_BUILD_PROGRAM_FAILURE:
			result_str = "CL_BUILD_PROGRAM_FAILURE";
			break;
		case CL_COMPILE_PROGRAM_FAILURE:
			result_str = "CL_COMPILE_PROGRAM_FAILURE";
			break;
		case CL_COMPILER_NOT_AVAILABLE:
			result_str = "CL_COMPILER_NOT_AVAILABLE";
			break;
		case CL_DEVICE_NOT_FOUND:
			result_str = "CL_DEVICE_NOT_FOUND";
			break;
		case CL_DEVICE_NOT_AVAILABLE:
			result_str = "CL_DEVICE_NOT_AVAILABLE";
			break;
		case CL_DEVICE_PARTITION_FAILED:
			result_str = "CL_DEVICE_PARTITION_FAILED";
			break;
		case CL_EXEC_STATUS_ERROR_FOR_EVENTS_IN_WAIT_LIST:
			result_str = "CL_EXEC_STATUS_ERROR_FOR_EVENTS_IN_WAIT_LIST";
			break;
		case CL_IMAGE_FORMAT_MISMATCH:
			result_str = "CL_IMAGE_FORMAT_MISMATCH";
			break;
		case CL_IMAGE_FORMAT_NOT_SUPPORTED:
			result_str = "CL_IMAGE_FORMAT_NOT_SUPPORTED";
			break;
		case CL_INVALID_ARG_INDEX:
			result_str = "CL_INVALID_ARG_INDEX";
			break;
		case CL_INVALID_ARG_SIZE:
			result_str = "CL_INVALID_ARG_SIZE";
			break;
		case CL_INVALID_ARG_VALUE:
			result_str = "CL_INVALID_ARG_VALUE";
			break;
		case CL_INVALID_BINARY:
			result_str = "CL_INVALID_BINARY";
			break;
		case CL_INVALID_BUFFER_SIZE:
			result_str = "CL_INVALID_BUFFER_SIZE";
			break;
		case CL_INVALID_BUILD_OPTIONS:
			result_str = "CL_INVALID_BUILD_OPTIONS";
			break;
		case CL_INVALID_COMMAND_QUEUE:
			result_str = "CL_INVALID_COMMAND_QUEUE";
			break;
		case CL_INVALID_COMPILER_OPTIONS:
			result_str = "CL_INVALID_COMPILER_OPTIONS";
			break;
		case CL_INVALID_CONTEXT:
			result_str = "CL_INVALID_CONTEXT";
			break;
		case CL_INVALID_DEVICE:
			result_str = "CL_INVALID_DEVICE";
			break;
		case CL_INVALID_DEVICE_PARTITION_COUNT:
			result_str = "CL_INVALID_DEVICE_PARTITION_COUNT";
			break;
		case CL_INVALID_DEVICE_QUEUE:
			result_str = "CL_INVALID_DEVICE_QUEUE";
			break;
		case CL_INVALID_DEVICE_TYPE:
			result_str = "CL_INVALID_DEVICE_TYPE";
			break;
		case CL_INVALID_EVENT:
			result_str = "CL_INVALID_EVENT";
			break;
		case CL_INVALID_EVENT_WAIT_LIST:
			result_str = "CL_INVALID_EVENT_WAIT_LIST";
			break;
		case CL_INVALID_GLOBAL_OFFSET:
			result_str = "CL_INVALID_GLOBAL_OFFSET";
			break;
		case CL_INVALID_GLOBAL_WORK_SIZE:
			result_str = "CL_INVALID_GLOBAL_WORK_SIZE";
			break;
		case CL_INVALID_HOST_PTR:
			result_str = "CL_INVALID_HOST_PTR";
			break;
		case CL_INVALID_IMAGE_DESCRIPTOR:
			result_str = "CL_INVALID_IMAGE_DESCRIPTOR";
			break;
		case CL_INVALID_IMAGE_FORMAT_DESCRIPTOR:
			result_str = "CL_INVALID_IMAGE_FORMAT_DESCRIPTOR";
			break;
		case CL_INVALID_IMAGE_SIZE:
			result_str = "CL_INVALID_IMAGE_SIZE";
			break;
		case CL_INVALID_KERNEL:
			result_str = "CL_INVALID_KERNEL";
			break;
		case CL_INVALID_KERNEL_ARGS:
			result_str = "CL_INVALID_KERNEL_ARGS";
			break;
		case CL_INVALID_KERNEL_DEFINITION:
			result_str = "CL_INVALID_KERNEL_DEFINITION";
			break;
		case CL_INVALID_KERNEL_NAME:
			result_str = "CL_INVALID_KERNEL_NAME";
			break;
		case CL_INVALID_LINKER_OPTIONS:
			result_str = "CL_INVALID_LINKER_OPTIONS";
			break;
		case CL_INVALID_MEM_OBJECT:
			result_str = "CL_INVALID_MEM_OBJECT";
			break;
		case CL_INVALID_OPERATION:
			result_str = "CL_INVALID_OPERATION";
			break;
		case CL_INVALID_PIPE_SIZE:
			result_str = "CL_INVALID_PIPE_SIZE";
			break;
		case CL_INVALID_PLATFORM:
			result_str = "CL_INVALID_PLATFORM";
			break;
		case CL_INVALID_PROGRAM:
			result_str = "CL_INVALID_PROGRAM";
			break;
		case CL_INVALID_PROGRAM_EXECUTABLE:
			result_str = "CL_INVALID_PROGRAM_EXECUTABLE";
			break;
		case CL_INVALID_PROPERTY:
			result_str = "CL_INVALID_PROPERTY";
			break;
		case CL_INVALID_QUEUE_PROPERTIES:
			result_str = "CL_INVALID_QUEUE_PROPERTIES";
			break;
		case CL_INVALID_SAMPLER:
			result_str = "CL_INVALID_SAMPLER";
			break;
		case CL_INVALID_SPEC_ID:
			result_str = "CL_INVALID_SPEC_ID";
			break;
		case CL_INVALID_VALUE:
			result_str = "CL_INVALID_VALUE";
			break;
		case CL_INVALID_WORK_DIMENSION:
			result_str = "CL_INVALID_WORK_DIMENSION";
			break;
		case CL_INVALID_WORK_GROUP_SIZE:
			result_str = "CL_INVALID_WORK_GROUP_SIZE";
			break;
		case CL_INVALID_WORK_ITEM_SIZE:
			result_str = "CL_INVALID_WORK_ITEM_SIZE";
			break;
		case CL_KERNEL_ARG_INFO_NOT_AVAILABLE:
			result_str = "CL_KERNEL_ARG_INFO_NOT_AVAILABLE";
			break;
		case CL_LINK_PROGRAM_FAILURE:
			result_str = "CL_LINK_PROGRAM_FAILURE";
			break;
		case CL_LINKER_NOT_AVAILABLE:
			result_str = "CL_LINKER_NOT_AVAILABLE";
			break;
		case CL_MAP_FAILURE:
			result_str = "CL_MAP_FAILURE";
			break;
		case CL_MEM_COPY_OVERLAP:
			result_str = "CL_MEM_COPY_OVERLAP";
			break;
		case CL_MEM_OBJECT_ALLOCATION_FAILURE:
			result_str = "CL_MEM_OBJECT_ALLOCATION_FAILURE";
			break;
		case CL_MISALIGNED_SUB_BUFFER_OFFSET:
			result_str = "CL_MISALIGNED_SUB_BUFFER_OFFSET";
			break;
		case CL_OUT_OF_HOST_MEMORY:
			result_str = "CL_OUT_OF_HOST_MEMORY";
			break;
		case CL_OUT_OF_RESOURCES:
			result_str = "CL_OUT_OF_RESOURCES";
			break;
		case CL_MAX_SIZE_RESTRICTION_EXCEEDED:
			result_str = "CL_MAX_SIZE_RESTRICTION_EXCEEDED";
			break;
		case CL_PROFILING_INFO_NOT_AVAILABLE:
			result_str = "CL_PROFILING_NOT_AVAILABLE";
			break;
		case CL_INVALID_GL_OBJECT:
			result_str = "CL_INVALID_GL_OBJECT";
			break;
		default:
            result_str = "Unknown error";
            break;
    }
    return result_str;
}