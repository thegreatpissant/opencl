#define __CL_ENABLE_EXCEPTIONS

#include <iostream>
#include <vector>
#include <string>
#include <unistd.h>
#include <CL/cl.hpp>

using namespace std;


std::string program_float = "__kernel void memcopy( __constant float16 *a, __global float16 *b) { int i = get_global_id(1); b[i] = get_global_id(0);}";

int main () 
{
  vector <cl::Platform> platforms;
  vector <cl::Device>   platformDevices, allDevices, ctxDevices;

  try {
    //  Platform: create
    cl::Platform::get(&platforms);
    platforms[0].getDevices (CL_DEVICE_TYPE_ALL, &platformDevices);
    //  Context: create
    cl::Context context (platformDevices);
    //  Device: recieve from context
    ctxDevices = context.getInfo <CL_CONTEXT_DEVICES> ();
    cout << "Work dimensions: " <<     ctxDevices[0].getInfo<CL_DEVICE_MAX_WORK_ITEM_DIMENSIONS> () << endl;
    //  Program create from source text
    cl::Program::Sources source (1, make_pair(program_float.c_str(), program_float.length() + 1));
    cl::Program program (context, source);
    //  Program: build
    try {
      program.build (ctxDevices);
    } 
    catch (cl::Error & err) {
      cout << err.what() << ": Error code "
           << err.err () << endl
           << program.getBuildInfo <CL_PROGRAM_BUILD_LOG>(ctxDevices[0])
           << endl;
    }
    //  Kernel create from program
    cl::Kernel kernel (program, "memcopy");
    //  Data: define 
    #define NUM_ITEMS 16
    cl_float a[NUM_ITEMS], b[NUM_ITEMS];
    for (auto &i:a)
      i = 0.0f;
    for (auto &i:b)
      i = 0.0f;
    //  Buffers: create and populate
    cl::Buffer ba (context, CL_MEM_READ_ONLY|CL_MEM_COPY_HOST_PTR, sizeof (a), a);
    cl::Buffer bb (context, CL_MEM_WRITE_ONLY, sizeof (b), NULL);
    
    //  Buffers: grab some information about 
    cout << "Buffer ba" << endl
         << "size: " << ba.getInfo<CL_MEM_SIZE>() << endl;
    cout << "Buffer bb" << endl
         << "size: " << bb.getInfo<CL_MEM_SIZE>() << endl;
    //  Kernel set arguments
    kernel.setArg (0, ba);
    kernel.setArg (1, bb);
    //  Command Queue: create 
    cl::CommandQueue cq (context, ctxDevices[0]);
    //    cq.enqueueWriteBuffer (ba, CL_TRUE, 0, sizeof (a), a);
/*
    //  Command Queue: enqueue kernel
    cq.enqueueTask (kernel);
*/
    //  Command Queue: create a range queue
    //    sleep(1);
    cl::NDRange offset (0);
    cl::NDRange global_size (3);
    cl::NDRange local_size (3);
    cq.enqueueNDRangeKernel (kernel, offset, global_size, local_size);
    cq.enqueueReadBuffer (bb, CL_TRUE, 0, sizeof (b), &b);

    cout << "Buffer bb read back: " << endl;
    for (auto i = 0; i < NUM_ITEMS; ++i) {
      cout << "a[" << i << "] == " << a[i] << "\t"
           << "b[" << i << "] == " << b[i] << endl;
    }
    
  }
  catch (cl::Error e) {
    cout << e.what () << ": Error code "
         << e.err () << endl;
  }

  ctxDevices.erase(ctxDevices.begin(), ctxDevices.end());
  return 0;
}
