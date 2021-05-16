#include "../src/DeviceProvider.hpp"
#include <sstream>

int main(int argc, char *argv[]) {
    try {
        auto devices = DeviceProvider::getAll();
        if (devices.empty()) {
            throw std::runtime_error("No devices found");
        }

        auto deviceInfo = devices[0];
        auto device = *deviceInfo.getCLDevice();

        cl::Context context(device);

        cl::Program::Sources sources;
        sources.emplace_back("kernel void sum(global const int *a, global const int *b, global int *c) {\n"
                             "  *c = *a + *b;\n"
                             "}");

        cl::Program program(context, sources);
        program.build();

        int a = 1124124;
        int b = -1242151;
        int c;
        cl::Buffer buffer_a(context, CL_MEM_READ_ONLY | CL_MEM_HOST_WRITE_ONLY, sizeof(int));
        cl::Buffer buffer_b(context, CL_MEM_READ_ONLY | CL_MEM_HOST_WRITE_ONLY, sizeof(int));
        cl::Buffer buffer_c(context, CL_MEM_WRITE_ONLY | CL_MEM_HOST_READ_ONLY, sizeof(int));

        cl::Kernel sum(program, "sum");
        sum.setArg(0, buffer_a);
        sum.setArg(1, buffer_b);
        sum.setArg(2, buffer_c);

        cl::CommandQueue queue(context, device);
        queue.enqueueWriteBuffer(buffer_a, CL_FALSE, 0, sizeof(int), &a);
        queue.enqueueWriteBuffer(buffer_b, CL_FALSE, 0, sizeof(int), &b);
        queue.enqueueNDRangeKernel(sum, cl::NullRange, cl::NDRange(3), cl::NullRange);
        queue.finish();

        queue.enqueueReadBuffer(buffer_c, CL_TRUE, 0, sizeof(int), &c);
        int d = 1;
    } catch (const cl::BuildError &e) {
        std::ostringstream buildErrorLog;
        for (auto &error: e.getBuildLog()) {
            buildErrorLog << error.second << std::endl;
        }
        throw std::runtime_error("Build error: \n" + buildErrorLog.str());
    }
    catch (const cl::Error &e) {
        auto clError = clGetErrorString(e.err());
        throw std::runtime_error("OpenCL error " + std::string(e.what()) + " " + clError);
    }
    catch (const std::exception &e) {
        std::fprintf(stderr, "The program was aborted: %s", e.what());
        return 1;
    }

    return 0;
}