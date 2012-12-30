/*
 * James A. Feister openjaf@gmail.com
 * OpenCL operating on vectors in a kernel.
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
using std::string;

int main (int argc, char * argv[])
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
  char fname[] = "ch5_vector_test.cl";
  string *fbuffer = new string;
  size_t program_size;
  cl_program program;
  cl_int program_ret;
  
  if ( (program_size = sb_clReadSourceProgramFromDisk ( fname, fbuffer ) ) < 0 ) {
    EXIT_FAIL;
  }
  program = sb_clCreateProgramFromSource ( fbuffer, context );
  if (program == NULL) {
    EXIT_FAIL;
  }
  program_ret = sb_clBuildProgram (&program, 1, device, NULL, NULL, NULL);
  if ( program_ret < 0) {
    EXIT_FAIL;
  }

  /*
   * Create kernel
   */
  cl_kernel kernel;
  char kernel_name[] = "vector_kernel";
  if ( (kernel = sb_clCreateKernel (program, kernel_name)) == NULL ) {
    EXIT_FAIL;
  }

  /*
   * Command Queue
   */
  cl_command_queue command_queue;
  if (( command_queue = sb_clCreateCommandQueue (context, *device, CL_QUEUE_OUT_OF_ORDER_EXEC_MODE_ENABLE)) == NULL) {
    EXIT_FAIL;
  }

  /*
   * set arguments
   */
  cl_int vect[4];
  cl_mem vect_buffer;
  vect_buffer = sb_clCreateBuffer (context, CL_MEM_WRITE_ONLY, sizeof(vect), NULL);
  if (vect_buffer == NULL) {
    cerr << "Failed to wrap vect argument" << endl;
    EXIT_FAIL;
  }

  if (sb_clSetKernelArg (kernel, 0, sizeof(cl_mem), &vect_buffer) != CL_SUCCESS) {
    cerr << "Failed to set kernel arg" << endl;
    EXIT_FAIL;
  }

    //  Enqueue Kernel
  cl_event vect_event;
  if (sb_clEnqueueTask (command_queue, kernel, 0, NULL, &vect_event) != CL_SUCCESS ) {
    EXIT_FAIL;
  } else {
    cout << "Kernel was enqueued" << endl;
  }

  clWaitForEvents (1, &vect_event);

  //  Read the data
  if (sb_clEnqueueReadBuffer (command_queue, vect_buffer, CL_TRUE, 0, sizeof (vect), &vect, 0, NULL, NULL) != CL_SUCCESS) {
    cerr << "Failed to read vect." << endl;
  }


  cout << "Vector 4 returned: [" 
       << vect[0] << "][" 
       << vect[1] << "][" 
       << vect[2] << "][" 
       << vect[3] << "]" << endl;

  // CleanUP
  clReleaseMemObject (vect_buffer);
  clReleaseCommandQueue (command_queue);
  clReleaseKernel  (kernel);
  clReleaseProgram (program);
  clReleaseContext (context);

  delete fbuffer;
  //  Generic section vars
  delete device;
  delete platform;
}
