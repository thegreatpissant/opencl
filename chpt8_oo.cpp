#define __CL_ENABLE_EXCEPTIONS

#include <iostream>
#include <vector>
#include <string>

#include <CL/cl.hpp>

using namespace std;

int main () 
{
  vector <cl::Platform> platforms;
  vector <cl::Device>   platformDevices, allDevices, ctxDevices;

  try {
    cl::Platform::get(&platforms);
    platforms[0].getDevices (CL_DEVICE_TYPE_ALL, &platformDevices);

    cl::Context context (platformDevices);
    ctxDevices = context.getInfo <CL_CONTEXT_DEVICES> ();
    for (cl_uint i = 0; i < ctxDevices.size(); i++) { 
      cout << "Device: " 
           << ctxDevices[i].getInfo <CL_DEVICE_NAME>()
           << endl;
    }
  }
  catch (cl::Error e) {
    cout << e.what() << ": Error code "
         << e.err () << endl;
  }

  return 0;
}
