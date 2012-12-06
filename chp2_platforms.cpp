/*
 * James A. Feister openjaf@gmail.com 
 *
 * Step by step for setting up an opencl application using the
 * sb_opencl.h helper functions.
 */
#include <iostream>
#include <cstdlib>
#include <CL/cl.h>

#include "sb_opencl.h"

//  Some shortcuts for typing
#define EXIT_FAIL exit ( EXIT_FAILURE )
#define EXIT_SUCC exit ( EXIT_SUCCESS )

using std::cout;
using std::cerr;
using std::endl;

int main ()
{
  
  cl_uint num_platforms;  
  cl_platform_id * platforms;
  cl_uint platform_id = 0;
  cl_uint num_devices;
  cl_device_id * devices;
  cl_uint device_id = 0;  
  cl_context context;
  cl_int context_error_ret;

  /*
   * Step 1, get platform
   */
  if ( clGetPlatformIDs (1, NULL, &num_platforms) < 0 ) {
    cerr << "Could not get number of platforms." << endl;
    EXIT_FAIL;
  }

  cout << "Found " << num_platforms << " platform" << (num_platforms > 1 ? "s": "") << endl;

  if ( num_platforms == 0 ) {
    cerr << "Did not find possible platforms." << endl;
    EXIT_FAIL;
  }

  platforms = (cl_platform_id *) malloc (sizeof (cl_platform_id) * num_platforms);
  if ( platforms == NULL ) {
    cerr << "Error allocating memory for platform" << endl;
    EXIT_FAIL;
  }

  if ( clGetPlatformIDs (num_platforms, platforms, NULL) < 0 ) {
    cerr << "Could not allocate the platforms needed." << endl;
    EXIT_FAIL;
  }

  sb_clPrintPlatformInfo (&platforms[platform_id]);

  /*
   * Step 2, get devices
   */
  if ( clGetDeviceIDs ( platforms[platform_id], CL_DEVICE_TYPE_ALL, 0, NULL, &num_devices) < 0 ) {
    cerr << "Could not query any devices for platform." << endl;
    EXIT_FAIL;
  }
  
  if (num_devices < 1) {
    cerr << "Did not find any CL_DEVICE_TYPE_ALL on the platform." << endl;
    EXIT_FAIL;
  }
  
  //  Keep this to one device for now.
  num_devices = 1;

  devices = (cl_device_id *) malloc (sizeof (cl_device_id)*num_devices);
  if ( devices == NULL) {
    cerr << "Could not allocate memory for devices." << endl;
    EXIT_FAIL;
  }
  
  if ( clGetDeviceIDs (platforms[platform_id], CL_DEVICE_TYPE_ALL,
		       num_devices, devices, NULL) < 0 ) {
    cerr << "Could not get the device requested" << endl;
    EXIT_FAIL;
  }

  sb_clPrintDeviceInfo ( &devices[device_id] );


  /*
   * Step 3, context
   */
  context = clCreateContext ( NULL,
			      1, devices,
			      NULL, NULL, &context_error_ret);

  if ( ! (context != 0 && context_error_ret == CL_SUCCESS )) {
    cerr << "Error creating the context." << endl;
    EXIT_FAIL;
  }

  /*
   * Step 4, The Program
   */
  char fname[] = "cl3.cl";
  std::string * fbuffer = new std::string;
  size_t program_size;
  cl_program program;
  cl_int program_ret;
  
  sb_clReadSourceProgramFromDisk ( fname, fbuffer);
  cout  << "Program read: " << *fbuffer << endl;
  program = sb_clCreateProgramFromSource ( fbuffer, context );
  if (program == NULL) {
    EXIT_FAIL;
  }
  program_ret = sb_clBuildProgram (&program, 1, devices, NULL, NULL, NULL);
  if (program_ret < 0) {
    EXIT_FAIL;
  }

  /*
   * Step 5, Create kernels for functions
   */
  cl_kernel kernel;

  if ( NULL == (kernel = sb_clCreateKernel (program, "good")) ) {
    EXIT_FAILURE;
  } else {
    cout << "Got kernel for function \'good\'." << endl;
  }

  /*
   * Step 6, Create Command Queues
   */
  cl_command_queue command_queue;

  if ( NULL == (command_queue = sb_clCreateCommandQueue (context, devices[0], CL_QUEUE_OUT_OF_ORDER_EXEC_MODE_ENABLE)) ) {
    EXIT_FAILURE;
  } else {
    cout << "Got command queue from device and context requested. " << endl;
  }

  /*
   * Step 7, set the args
   */
  //  Declare the data
  cl_float a = 3, b = 2, c = 0;
  cl_mem ka, kb, kc;
  cl_int buffer_error_code;
  //  Wrap the data
  ka = sb_clCreateBuffer (context, CL_MEM_READ_ONLY|CL_MEM_COPY_HOST_PTR,
			  sizeof (cl_float), &a);
  if (ka == NULL) {
    cerr << "Failed to wrap arg a" << endl;
  }
  kb = sb_clCreateBuffer (context, CL_MEM_READ_ONLY|CL_MEM_COPY_HOST_PTR,
			  sizeof (cl_float), &b);
  if (kb == NULL) {
    cerr << "Failed to wrap arg b" << endl;
  }
  kc = sb_clCreateBuffer (context, CL_MEM_WRITE_ONLY, sizeof (cl_float), NULL);
  if (kc == NULL) {
    cerr << "Failed to wrap arg c" << endl;
  }
  if (ka == NULL || kb == NULL || kc == NULL) {
    EXIT_FAIL;
  }

  if (sb_clSetKernelArg (kernel, 0, sizeof (cl_mem), &ka) != CL_SUCCESS) {
    cerr << "Failed to set kernel arg 0" << endl;
    EXIT_FAIL;
  }
  if (sb_clSetKernelArg (kernel, 1, sizeof (cl_mem), &kb) != CL_SUCCESS) {
    cerr << "Failed to set kernel arg 1" << endl;
    EXIT_FAIL;
  }  
  if (sb_clSetKernelArg (kernel, 2, sizeof (cl_mem), &kc) != CL_SUCCESS) {
    cerr << "Failed to set kernel arg 2" << endl;
    EXIT_FAIL;
  }

  /*
   * Step 8, Enque the kernel
   */
  cl_int task_error;
  if (sb_clEnqueueTask (command_queue, kernel, 0, NULL, NULL) != CL_SUCCESS ) {
    EXIT_FAIL;
  } else {
    cout << "Kernel was enqueued" << endl;
  }

  /*
   *  Step 9, Read back our data
   */
  cl_int buffer_read_error;

  if (sb_clEnqueueReadBuffer (command_queue, kc, CL_TRUE, 0, sizeof (c), &c, 0, NULL, NULL) != CL_SUCCESS) {
    cerr << "Failed to enque readbuffer for c." << endl;
  }

  cout << "C is now: " << c << endl;
  cout << "kc is now: " << kc << endl;

  cout << "Mem object 1" << endl;
  sb_clGetMemObjectInfo (ka);
  cout << "Mem object 2" << endl;
  sb_clGetMemObjectInfo (kb);
  cout << "Mem object 3" << endl;
  sb_clGetMemObjectInfo (kc);

  clReleaseMemObject (ka);  
  clReleaseMemObject (kb);  
  clReleaseMemObject (kc);

  clReleaseCommandQueue (command_queue);
  clReleaseKernel  (kernel);
  clReleaseProgram (program);
  clReleaseContext (context);
}
