/*
n * James A. Feister openjaf@gmail.com
 *
 * "OpenCL in Action" populating vector data from a kernel.
 */

#include <iostream>
#include <cstdlib>
#include <CL/cl.h>

#include "sb_opencl.h"

//  defines
#define EXIT_FAIL exit ( EXIT_FAILURE )
#define EXIT_SUCC exit ( EXIT_SUCCESS )

//  namespaces
using std::cout;
using std::cerr;
using std::endl;

//  main

int main (int argc, char * argv[] )
{

  //  @@  GENERIC SECTION BEGIN  @@
  cl_uint num_platforms;
  cl_platform_id * platforms;
  cl_uint plafrom_id = 0;
  cl_uint num_devices;
  cl_device_id * devices;
  cl_uint device_id = 0;
  cl_context context;
  cl_int context_error_ret;

  /*
   * Get platform
   */
  if (clGetPlatformIDs (1, NULL, &num_platforms) < 0 ) {
    cerr << "Could not get number of platforms." << endl;
    EXIT_FAIL;
  }
  
  if ( num_platforms > 0 ) {
    cout << "Found " << num_platforms << " platform" << (num_platforms > 1 ? "s" : "") << endl;
  }
  else {
    cerr << "Did not find possible platforms" << endl;
    EXIT_FAIL;
  }

  platforms = new cl_platform_id;
  if (platforms == NULL ) {
    cerr << "Error allocating memory for platform." << endl;
    EXIT_FAIL;
  }
  
  if ( clGetPlatformIDs (1, platforms, NULL) < 0 ) {
    cerr << "Error allocating platform." << endl;
    EXIT_FAIL;
  }

  sb_clPrintPlatformInfo (platforms);


  /*
   * Get Device
   */
  if ( clGetDeviceIDs ( *platforms, CL_DEVICE_TYPE_ALL, 0, NULL, &num_devices ) < 0 ) {
    cerr << "Failed to query for platform devices." << endl;
    EXIT_FAIL;
  }

  if (num_devices > 0) {
    cout << "Found " << num_devices << " device" << (num_devices > 1 ? "s":"") << "." << endl;
    devices = new cl_device_id;
  } else {
    cerr << "Did not find any devices to use." << endl;
    EXIT_FAIL;
  }
  
  if ( clGetDeviceIDs (*platforms, CL_DEVICE_TYPE_ALL,
		       1, devices, NULL) < 0) {
    cerr << "Failed to get requested device." << endl;
    EXIT_FAIL;
  }
  sb_clPrintDeviceInfo ( devices );

  /*
   * Get context
   */
  context = clCreateContext ( NULL,
			      1, devices,
			      NULL, NULL, &context_error_ret );
  
  if ( ! (context != 0 && context_error_ret == CL_SUCCESS)) {
    cerr << "Error creating a context." << endl;
    EXIT_FAIL;
  }
  
  //  @@  GENERIC SECTION END  @@ 

  /*
   * Compile program
   */
  char fname[] = "hello_kernel.cl";
  std::string *fbuffer = new std::string;
  size_t program_size;
  cl_program program;
  cl_int program_ret;

  if ( sb_clReadSourceProgramFromDisk ( fname, fbuffer ) < 0 ) {
    EXIT_FAIL;
  }
  cout << "Program read: " << *fbuffer << endl;
  program = sb_clCreateProgramFromSource ( fbuffer, context );
  if (program == NULL) {
    EXIT_FAIL;
  }
  program_ret = sb_clBuildProgram (&program, 1, devices, NULL, NULL, NULL);
  if ( program_ret < 0) {
    EXIT_FAIL;
  }

  //  Get the kernel
  cl_kernel kernel;
  char kernel_name[] = "hello_kernel";

  if ( (kernel = sb_clCreateKernel (program, kernel_name)) == NULL ) {
    EXIT_FAILURE;
  } else {
    cout << "Kernelized function \'" << kernel_name << "\'" << endl;
  }

  //  Get Command Queue
  cl_command_queue command_queue;
  if ( (command_queue = sb_clCreateCommandQueue (context, *devices, CL_QUEUE_OUT_OF_ORDER_EXEC_MODE_ENABLE)) == NULL ) {
    EXIT_FAILURE;
  } else {
    cout << "Got command queue." << endl;
  }

  //  Set args
  char msg[16];
  cl_mem msg_buffer;
  cl_int msg_err;
  msg_buffer = sb_clCreateBuffer (context, CL_MEM_WRITE_ONLY, sizeof(msg), NULL);
  //msg_buffer = clCreateBuffer (context, CL_MEM_WRITE_ONLY, sizeof(msg), NULL, &msg_err);
  if (msg_buffer == NULL ) {
    cerr << "Failed to wrap msg_buffer" << endl;
    EXIT_FAIL;
  }

  if (sb_clSetKernelArg (kernel, 0, sizeof(cl_mem), &msg_buffer) != CL_SUCCESS) {
    cerr << "Failed to set kernel arg" << endl;
    EXIT_FAIL;
  }

  //  Enqueue Kernel
  cl_int task_error;
  if (sb_clEnqueueTask (command_queue, kernel, 0, NULL, NULL) != CL_SUCCESS ) {
    EXIT_FAIL;
  } else {
    cout << "Kernel was enqueued" << endl;
  }

  //  Read the data
  cl_int buffer_read_error;
  if (sb_clEnqueueReadBuffer (command_queue, msg_buffer, CL_TRUE, 0, sizeof (msg), &msg, 0, NULL, NULL) != CL_SUCCESS) {
    cerr << "Failed to read buffer msg." << endl;
  }

  cout << "message: " << msg << endl;

  clReleaseMemObject (msg_buffer);
  clReleaseCommandQueue (command_queue);
  clReleaseKernel  (kernel);
  clReleaseProgram (program);
  clReleaseContext (context);
  // CleanUP
  delete fbuffer;
  //  Generic section vars
  delete devices;
  delete platforms;
}