#include <iostream>
#include <cstdlib>
#include <CL/cl.h>

#include "sb_opencl.h"

#define EXIT_FAIL exit ( EXIT_FAILURE )
#define EXIT_SUCC exit ( EXIT_SUCCESS )

using std::cout;
using std::cerr;
using std::endl;

int main ()
{

  cl_uint num_platforms;
  cl_platform_id *platforms;
  cl_uint num_devices;
  cl_device_id *devices;

  if ( clGetPlatformIDs (1, NULL, &num_platforms) < 0 ) {
    cerr << "Could not get number of platforms." << endl;
    EXIT_FAIL;
  }

  if (num_platforms == 0) {
    cerr << "No platforms available" << endl;
    EXIT_FAIL;
  }

  platforms = new cl_platform_id[num_platforms];
  if (clGetPlatformIDs (num_platforms, platforms, NULL) < 0 ) {
    cerr << "Error getting platforms" << endl;
    EXIT_FAIL;
  }
  cout << "Got " << num_platforms << " Platform" << (num_platforms > 1 ?"s" : "") << endl;

  for (cl_uint i = 0; i < num_platforms; ++i) {
    cout << "Testing Platform #" << i << endl;
    cl_platform_id cur_platform = platforms[i];
    sb_clPrintPlatformInfo (&cur_platform);

    //  Get Device count
    if ( clGetDeviceIDs (cur_platform, CL_DEVICE_TYPE_ALL, 0, NULL, &num_devices) < 0 ) {
      cerr << "Failed to query for devices" << endl;
      EXIT_FAIL;
    }

    if (num_devices == 0) {
      cerr << "Did not get any devices" << endl;
      EXIT_FAIL;
    }

    if (num_devices == 1) {
      cout << "Single device test" << endl;
      devices = new cl_device_id[1];
      if (clGetDeviceIDs (cur_platform, CL_DEVICE_TYPE_ALL, 1, devices, NULL) < 0) {
	cerr << "Could not get the device" << endl;
	EXIT_FAIL;
      }
      cl_device_id cur_device = devices[0];
      //      sb_clPrintDeviceInfo (&cur_device);
      
      cl_ulong dev_mem_size;
      clGetDeviceInfo (cur_device, CL_DEVICE_GLOBAL_MEM_SIZE, sizeof (cl_ulong), &dev_mem_size, NULL);
      cout << "Working with " << dev_mem_size << " bytes of memory." << endl;
    }
  }
  
}
