#include <stdio.h>
#include <stdlib.h>

#include <CL/cl.h>

int main() {
    int result;
    unsigned int num_platforms, num_devices;
    size_t param_size;

    if((result = clGetPlatformIDs(0, NULL, &num_platforms)) != CL_SUCCESS) {
        printf("Error querying platforms: clGetPlatformIDs(%d)\n", result);
        return -1;
    }

    if(num_platforms == 0) {
        printf("No platforms found\n");
        return 0;
    }

    printf("%d platform(s) found\n", num_platforms);

    cl_platform_id *all_platforms = (cl_platform_id *)malloc(num_platforms * sizeof(cl_platform_id));

    if((result = clGetPlatformIDs(num_platforms, all_platforms, NULL)) != CL_SUCCESS) {
        printf("Error querying platforms: clGetPlatformIDs(%d)\n", result);
        free(all_platforms);
        return -1;
    }

    cl_platform_id default_platform = all_platforms[0];

    if((result = clGetPlatformInfo(default_platform, CL_PLATFORM_NAME, 0, NULL, &param_size)) != CL_SUCCESS) {
        printf("Error querying platforms: clGetPlatformInfo(%d)\n", result);
        free(all_platforms);
        return -1;
    }

    char *platform_name = (char *)malloc(param_size);

    if((result = clGetPlatformInfo(default_platform, CL_PLATFORM_NAME, param_size, platform_name, 0)) != CL_SUCCESS) {
        printf("Error querying platforms: clGetPlatformInfo(%d)\n", result);
        free(all_platforms);
        free(platform_name);
        return -1;
    }

    printf("Default platform name: %s\n", platform_name);

    if((result = clGetDeviceIDs(default_platform, CL_DEVICE_TYPE_CPU | CL_DEVICE_TYPE_GPU, 0, NULL, &num_devices)) != CL_SUCCESS) {
        printf("Error querying devices: clGetDeviceIds(%d)\n", result);
        free(all_platforms);
        free(platform_name);
        return -1;
    }

    printf("%d device(s) found\n", num_devices);

    cl_device_id *all_devices = (cl_device_id *)malloc(num_devices * sizeof(cl_device_id));

    if((result = clGetDeviceIDs(default_platform, CL_DEVICE_TYPE_CPU | CL_DEVICE_TYPE_GPU, num_devices, all_devices, NULL)) != CL_SUCCESS) {
        printf("Error querying devices: clGetDeviceIds(%d)\n", result);
        free(all_platforms);
        free(platform_name);
        free(all_devices);
        return -1;
    }

    cl_device_id default_device = all_devices[0];

    if((result = clGetDeviceInfo(default_device, CL_DEVICE_NAME, 0, NULL, &param_size)) != CL_SUCCESS) {
        printf("Error querying devices: clGetDeviceInfo(%d)\n", result);
        free(all_platforms);
        free(platform_name);
        free(all_devices);
        return -1;
    }

    char *device_name = (char *)malloc(param_size);

    if((result = clGetDeviceInfo(default_device, CL_DEVICE_NAME, param_size, device_name, 0)) != CL_SUCCESS) {
        printf("Error querying devices: clGetDeviceInfo(%d)\n", result);
        free(all_platforms);
        free(platform_name);
        free(all_devices);
        free(device_name);
        return -1;
    }

    printf("Default device name: %s\n", device_name);

    free(all_platforms);
    free(platform_name);
    free(all_devices);
    free(device_name);

    return 0;
}