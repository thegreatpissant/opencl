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

    cout << "Platform extensions: "
         << platforms[0].getInfo <CL_PLATFORM_EXTENSIONS> ()
         << endl;
    cl::Context context (platformDevices);
    ctxDevices = context.getInfo <CL_CONTEXT_DEVICES> ();

    for (auto device : ctxDevices) {
      cout << "Device Name: " 
           << device.getInfo <CL_DEVICE_NAME>()
           << endl;
      cout << "Device Extensions: "
           << device.getInfo <CL_DEVICE_EXTENSIONS>()
           << endl;
      /*      cout << "Device C Version: "
           << device.getInfo <CL_DEVICE_OPENCL_C_VERSION>()
           << endl;
      */
    }
  }
  catch (cl::Error e) {
    cout << e.what() << ": Error code "
         << e.err () << endl;
  }

  ctxDevices.erase(ctxDevices.begin(), ctxDevices.end());
  return 0;
}
