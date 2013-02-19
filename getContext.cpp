#include <iostream>
#include <vector>
#include <CL/cl.hpp>

using namespace std;

#ifndef CL_CALLBACK
#define CL_CALLBACK
#endif
 void CL_CALLBACK context_callback (const char * errinfo, 
                               const void *private_info_size, 
                               ::size_t cb, 
                               void *user_data ) {
  cout << "Recieved an error from context" << endl;
  cout << "Error: " << errinfo << endl;
}


int main () {
  
  vector <cl::Platform> Platforms;
  vector <cl::Device>   Devices, ctxDevices;

  cl::Platform::get(&Platforms);
  if (Platforms.size() == 0) {
    cerr << "No platform available" << endl;
    exit (EXIT_FAILURE);
  }
    
  Platforms[0].getDevices(CL_DEVICE_TYPE_ALL, &Devices);
  if (Devices.size() == 0) {
    cerr << "No devices available" << endl;
    exit (EXIT_FAILURE);
  }

  cl_int err;
  cl::Context::Context MyContext (Devices, NULL, context_callback, NULL, &err);
 if (err != CL_SUCCESS) {
   cerr << "Obtaining context failed" << endl;
   exit (EXIT_FAILURE);
 }
 err = MyContext.getInfo(CL_CONTEXT_DEVICES,&ctxDevices);
 if (err != CL_SUCCESS) {
   cerr << "Failure obtaining devices from context" << endl;
   exit (EXIT_FAILURE);
 }


}

