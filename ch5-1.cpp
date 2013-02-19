#define __CL_ENABLE_EXCEPTIONS

#include <iostream>
#include <vector>
#include <string>
#include <fstream>

#include <CL/cl.hpp>

#define CLKERNEL "ch5-1.cl"

int main ()
{

  using namespace std;

  vector <cl::Platform> platforms;
  vector <cl::Device > platformDevices, allDevices, ctxDevices;

  try {

    cl::Platform::get (&platforms);
    platforms[0].getDevices (CL_DEVICE_TYPE_ALL, &platformDevices);

    cl::Context Context (platformDevices);

    ctxDevices = Context.getInfo <CL_CONTEXT_DEVICES>();

    ifstream programFile (CLKERNEL);
    string programString (istreambuf_iterator<char> (programFile),
                          (istreambuf_iterator<char> ()));
    cl::Program::Sources Source (1, make_pair(programString.c_str(), programString.length() + 1 ));
    cl::Program::Program Program (Context, Source);
    try {
      Program.build (ctxDevices);
    }
    catch (cl::Error & err) {
      cout << err.what() << ": Error code " << err.err() << endl
           << Program.getBuildInfo <CL_PROGRAM_BUILD_LOG> (ctxDevices[0]) << endl;
    }

    cl::Kernel Kernel (Program, "op_test");

    #define NUM_ITEMS 4

    cl_int a[NUM_ITEMS];
    fill_n (a, NUM_ITEMS, -1);

    cl::Buffer bn (Context, CL_MEM_WRITE_ONLY, sizeof (a), NULL);

    Kernel.setArg (0, bn);

    cl::CommandQueue cq (Context, ctxDevices[0]);

    cq.enqueueTask (Kernel);
    cq.enqueueReadBuffer (bn, CL_TRUE, 0, sizeof (a), &a);
    cout << "Got back the buffer" << endl;

    for ( auto i = 0; i < NUM_ITEMS; ++i) {
      cout << "[" << i << "] == " << a[i] << endl;
    }
  }
  catch (cl::Error err) {
    cout << err.what() << " : Error code " << err.err () << endl;
  }
}
