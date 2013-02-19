#define __CL_ENABLE_EXCEPTIONS

#include <iostream>
#include <fstream>
#include <vector>
#include <string>
#include <CL/cl.hpp>

using namespace std;

#define CLKERNEL "ch4-4-3.cl"

int main ()
{
  vector <cl::Platform> platforms;
  vector <cl::Device>   platformDevices, allDevices, ctxDevices;

  try {
    //  Platform
    cl::Platform::get (&platforms);
    platforms[0].getDevices (CL_DEVICE_TYPE_ALL, &platformDevices);

    //  Context
    cl::Context context (platformDevices);

    //  Device: recieve from Context
    ctxDevices = context.getInfo <CL_CONTEXT_DEVICES>();
    
    ifstream programFile (CLKERNEL);
    string programString (istreambuf_iterator<char> (programFile),
                          (istreambuf_iterator<char>()));
    cl::Program::Sources Source (1, make_pair(programString.c_str(), programString.length() +1));
    cl::Program Program (context, Source);

    try {
      Program.build (ctxDevices);
    }
    catch (cl::Error & err) {
      cout << err.what() << ": Error code "
           << err.err()  << endl
           << Program.getBuildInfo <CL_PROGRAM_BUILD_LOG>(ctxDevices[0])
           << endl;
    }

    cl::Kernel kernel (Program, "memstuff");

    #define NUM_ITEMS 16
    cl_char a[NUM_ITEMS];
    fill_n(a, NUM_ITEMS, ' ');
    
    cl::Buffer ba (context, CL_MEM_WRITE_ONLY, sizeof (a), NULL);
    
    kernel.setArg (0, ba);
    
    cl::CommandQueue cq (context, ctxDevices[0]);

    cq.enqueueTask (kernel);
    cq.enqueueReadBuffer (ba, CL_TRUE, 0, sizeof (a), &a);
    cout << "Got back the kernel information: " << a << endl;
  }
  catch (cl::Error e) {
    cout << e.what() << " : Error code "
         << e.err () << endl;
  }
}
