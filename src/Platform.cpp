#include "Platform.h"

std::vector<Platform> Platform::allPlatforms() {
    int result; //result of query
    unsigned int num_platforms; //number of available platforms

    std::vector<Platform> all_platforms;

    //get count of platforms
    if((result = clGetPlatformIDs(0, nullptr, &num_platforms)) != CL_SUCCESS)
        return all_platforms;

    //allocate memory for platform ids
    std::vector<cl_platform_id> all_platform_ids(num_platforms);

    //get platform ids
    if((result = clGetPlatformIDs(num_platforms, all_platform_ids.data(), nullptr)))
        return all_platforms;

    //create platforms
    for(unsigned int i = 0; i < num_platforms; i++)
        all_platforms.push_back(Platform(all_platform_ids[i]));

    return all_platforms;
}