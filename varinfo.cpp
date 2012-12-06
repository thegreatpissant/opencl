/*
 * James A. Feister openjaf@gmail.com 
 *
 * Print out information on all platform's devices variable
 * capabilities
 */
#include <iostream>
#include <string>
#include <cstdlib>
#include "sb_opencl.h"

using std::cout;
using std::cerr;
using std::endl;

#define EXIT_FAIL exit (EXIT_FAILURE)
#define EXIT_SUCC exit (EXIT_SUCCESS)

int main () 
{
  cout << "Gathering information about device specifics." << endl;

  cl_uint num_platforms;
  cl_uint num_devices;
  cl_platform_id *platforms;
  cl_device_id *devices;

  if (clGetPlatformIDs (1, NULL, &num_platforms) < 0) {
    cerr << "Error geting platforms available." << endl;
    EXIT_FAIL;
  }

  if (num_platforms == 0) {
    cerr << "Did not find any platforms." << endl;
    EXIT_FAIL;
  }

  platforms = new cl_platform_id[num_platforms];

  if (clGetPlatformIDs (num_platforms, platforms, NULL) < 0) {
    cerr << "Failed to obtains the platforms." << endl;
    EXIT_FAIL;
  }

  for (cl_uint i = 0; i < num_platforms; ++i) {
    cout << "Platform #" << i << " device information." << endl;
    if (clGetDeviceIDs (platforms[i], CL_DEVICE_TYPE_ALL, 0, NULL, &num_devices) < 0 ) {
      cerr << "Failed to query for devices." << endl;
      EXIT_FAIL;
    }

    if (num_devices < 1) {
      cerr << "Failed to find any devices for the platform." << endl;
      EXIT_FAIL;
    }

    devices = new cl_device_id[num_devices];
    if (clGetDeviceIDs (platforms[i], CL_DEVICE_TYPE_ALL, num_devices, devices, NULL) < 0) {
      cerr << "Failed to get devices." << endl;
      EXIT_FAIL;
    }

    cout << "Found " << num_devices << "device" << (num_devices > 1 ?"s":"") << "." << endl;
    for (cl_uint i = 0; i < num_devices; ++i) {
      cout << "Preferred width's." << endl;
      cl_uint var_width;
      clGetDeviceInfo (devices[i], CL_DEVICE_PREFERRED_VECTOR_WIDTH_CHAR,
			sizeof (var_width), &var_width, NULL);
      cout << "Preferred vector width in chars:\t" << var_width << endl;
      clGetDeviceInfo (devices[i], CL_DEVICE_PREFERRED_VECTOR_WIDTH_SHORT,
			sizeof (var_width), &var_width, NULL);
      cout << "Preferred vector width in shorts:\t" << var_width << endl;
      clGetDeviceInfo (devices[i], CL_DEVICE_PREFERRED_VECTOR_WIDTH_INT,
			sizeof (var_width), &var_width, NULL);
      cout << "Preferred vector width in ints:\t\t" << var_width << endl;
      clGetDeviceInfo (devices[i], CL_DEVICE_PREFERRED_VECTOR_WIDTH_LONG,
			sizeof (var_width), &var_width, NULL);
      cout << "Preferred vector width in longs:\t" << var_width << endl;
      clGetDeviceInfo (devices[i], CL_DEVICE_PREFERRED_VECTOR_WIDTH_FLOAT,
			sizeof (var_width), &var_width, NULL);
      cout << "Preferred vector width in floats:\t" << var_width << endl;
      clGetDeviceInfo (devices[i], CL_DEVICE_PREFERRED_VECTOR_WIDTH_DOUBLE,
			sizeof (var_width), &var_width, NULL);
      cout << "Preferred vector width in doubles:\t" << var_width << endl;
      #ifdef CL_DEVICE_PREFERRED_VECTOR_WIDTH_HALF
	clGetDeviceInfo (devices[i], CL_DEVICE_PREFERRED_VECTOR_WIDTH_HALF,
			sizeof (var_width), &var_width, NULL);
      cout << "Preferred vector width in halfs:\t" << var_width << endl;
	#endif
    }
  }

  EXIT_SUCC;
}
