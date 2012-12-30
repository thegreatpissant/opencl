/*
 * James A. Feister openjaf@gmail.com
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

  cl_platform_id * platform;
  cl_device_id * device;
  cl_context context;

  if ( !sb_clGetContextWithDevice (&platform, &device, &context) ) {
      cerr << "Unable to get a device." << endl;
      EXIT_FAIL;
    }
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
  program_ret = sb_clBuildProgram (&program, 1, device, NULL, NULL, NULL);
  if ( program_ret < 0) {
    EXIT_FAIL;
  }

  //  Get the kernel
  cl_kernel kernel;
  char kernel_name[] = "hello_kernel";

  if ( (kernel = sb_clCreateKernel (program, kernel_name)) == NULL ) {
    EXIT_FAIL;
  } else {
    cout << "Kernelized function \'" << kernel_name << "\'" << endl;
  }

  //  Get Command Queue
  cl_command_queue command_queue;
  if ( (command_queue = sb_clCreateCommandQueue (context, *device, CL_QUEUE_OUT_OF_ORDER_EXEC_MODE_ENABLE)) == NULL ) {
    EXIT_FAIL;
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
  cl_event msg_event;
  cl_int task_error;
  if (sb_clEnqueueTask (command_queue, kernel, 0, NULL, &msg_event) != CL_SUCCESS ) {
    EXIT_FAIL;
  } else {
    cout << "Kernel was enqueued" << endl;
  }

  clWaitForEvents (1, &msg_event);
  //  Read the data
  cl_int buffer_read_error;
  if (sb_clEnqueueReadBuffer (command_queue, msg_buffer, CL_TRUE, 0, sizeof (msg), &msg, 0, NULL, NULL) != CL_SUCCESS) {
    cerr << "Failed to read buffer msg." << endl;
  }

  cout << "Message from kernel: " << msg << endl;

  // CleanUP
  clReleaseMemObject (msg_buffer);
  clReleaseCommandQueue (command_queue);
  clReleaseKernel  (kernel);
  clReleaseProgram (program);
  clReleaseContext (context);

  delete fbuffer;
  //  Generic section vars
  delete device;
  delete platform;
}
