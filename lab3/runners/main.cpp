#include <CL/cl.h>

int main(int argc, char *argv[]) {

    cl_int errorCode;

    // get platforms
    cl_uint numPlatforms;
    errorCode = clGetPlatformIDs(0, nullptr, &numPlatforms);

    auto *platforms = new cl_platform_id[numPlatforms];
    errorCode = clGetPlatformIDs(numPlatforms, platforms, nullptr);

    // get devices

    return 0;
}