/*
 * James A. Feister openjaf@gmail.com
 *
 * This example creates an array of cl_float's initialized to 2.0f,
 * sends them via opencl to the devices where the kernel multiplys it
 * by two and assigns it to the other global array to return from the
 * device memory to the host machine.
 */

#include <iostream>
#include <cstdlib>
#include <cstring>
#include <CL/cl.h>

#include "sb_opencl.h"

#define EXIT_FAIL exit ( EXIT_FAILURE )
#define EXIT_SUCC exit ( EXIT_SUCCESS )

using std::cout;
using std::cerr;
using std::endl;

std::string program_float = "kernel void memcopy( __constant float *a, __global float *b) { int i = get_global_id(0); b[i] = a[i] * 2; }";
  

int main ()
{

  cl_uint num_platforms;
  cl_platform_id *platforms;
  cl_uint num_devices;
  cl_device_id *devices;

  /*
   * Get number of platforms
   */
  if ( clGetPlatformIDs (1, NULL, &num_platforms) < 0 ) {
    cerr << "Could not get number of platforms." << endl;
    EXIT_FAIL;
  }

  if (num_platforms == 0) {
    cerr << "No platforms available" << endl;
    EXIT_FAIL;
  }

  /*
   * retrive The platforms
   */
  platforms = new cl_platform_id[num_platforms];
  if (clGetPlatformIDs (num_platforms, platforms, NULL) < 0 ) {
    cerr << "Error getting platforms" << endl;
    EXIT_FAIL;
  }
  cout << "Got " << num_platforms << " Platform" << (num_platforms > 1 ?"s" : "") << endl;

  //  For all platforms found
  for (cl_uint i = 0; i < num_platforms; ++i) {
    cout << "Testing Platform #" << i+1 << endl;
    cl_platform_id cur_platform = platforms[i];
    sb_clPrintPlatformInfo (&cur_platform);

    /*
     *  Get Device count
     */
    if ( clGetDeviceIDs (cur_platform, CL_DEVICE_TYPE_ALL, 0, NULL, &num_devices) < 0 ) {
      cerr << "Failed to query for devices" << endl;
      EXIT_FAIL;
    }

    if (num_devices == 0) {
      cerr << "Did not get any devices" << endl;
      EXIT_FAIL;
    }

    /*
     * Get a device
     */
    if (num_devices == 1) {
      cout << "Single device test" << endl;
      devices = new cl_device_id[1];
      if (clGetDeviceIDs (cur_platform, CL_DEVICE_TYPE_ALL, 1, devices, NULL) < 0) {
	cerr << "Could not get the device" << endl;
	EXIT_FAIL;
      }
      cl_device_id cur_device = devices[0];
      sb_clPrintDeviceInfo (&cur_device);
      
      cl_ulong dev_mem_size;
      clGetDeviceInfo (cur_device, CL_DEVICE_GLOBAL_MEM_SIZE, sizeof (cl_ulong), &dev_mem_size, NULL);
      cout << "Working with " << dev_mem_size << " bytes of memory." << endl;


      /*
       * Get a context
       */
      cl_int context_error_ret;
      cl_context context = clCreateContext ( NULL,
					    1, &cur_device, 
					    NULL, NULL, &context_error_ret);
      if (context_error_ret != CL_SUCCESS) {
	cerr << "Error creating context for device" << endl;
	EXIT_FAIL;
      } else {
	cout << "Got context" << endl;
      }
      /*
       * Create Program
       */
      cl_program program = sb_clCreateProgramFromSource (&program_float, context);
      if (program == NULL) {
	EXIT_FAIL;
      }
      cl_int program_ret = sb_clBuildProgram (&program, 1, &cur_device, NULL, NULL, NULL);
      if (program_ret < 0) {
	EXIT_FAIL;
      }

      /*
       * Get kernel from program
       */
      cl_kernel kernel;
      char kernel_name[] = "memcopy";
      if (NULL == (kernel = sb_clCreateKernel (program, kernel_name) ) ) {
	EXIT_FAIL;
      } else {
	cout << "Got kernel " << kernel_name << endl;
      }
      
      /*
       * Create a command queue
       */
      cl_command_queue command_queue;
      if ( NULL == (command_queue = sb_clCreateCommandQueue (context, cur_device, CL_QUEUE_OUT_OF_ORDER_EXEC_MODE_ENABLE)) ) {
	EXIT_FAIL;
      } else {
	cout << "Got command queue." << endl;
      }

      // Create data to send over
      #define NUM_ITEMS 1024
      cl_float a[NUM_ITEMS], b[NUM_ITEMS];
      cl_mem ka, kb;
      cl_int buffer_error;

      for (int i = 0; i < NUM_ITEMS; ++i)
	a[i] = 2.0f;
      ka = sb_clCreateBuffer (context, CL_MEM_READ_ONLY|CL_MEM_COPY_HOST_PTR,
			      sizeof (a), a);
      if (ka == NULL) {
	cerr << "Failed to wrap arg a" << endl;
      }
      kb = sb_clCreateBuffer (context, CL_MEM_WRITE_ONLY, 
			      sizeof (b), NULL);
      if (kb == NULL) {
	cerr << "Failed to wrap arg b" << endl;
      }
      if (kb == NULL || kb == NULL) {
	EXIT_FAIL;
      }

      /*
       * Send mem_object's to the kernel
       */
       if (sb_clSetKernelArg (kernel,  0, sizeof (cl_mem), &ka) != CL_SUCCESS) {
	cerr << "Failed to send data to kernel arg \'a\'" << endl;
	EXIT_FAIL;
       }
       if (sb_clSetKernelArg (kernel,  1, sizeof (cl_mem), &kb) != CL_SUCCESS) {
	cerr << "Failed to send data to kernel arg \'b\'" << endl;
	EXIT_FAIL;
       }

       /*
	* Queue actions
	*/
       cl_event enqueue_event;
       size_t work_items[] = {2, NUM_ITEMS/2};
       size_t dim = 2;
       cout << "Waiting for kernel to complete ... ";
       if (CL_SUCCESS != clEnqueueNDRangeKernel (command_queue, kernel, 
						 dim, NULL,   // work_dim, global_work_offset
						 work_items, // global_work_size
						 NULL,  // local_work_size
						 0, NULL, NULL) // num_wait_event, events, event
	   ) {
	 cerr << "Failed to enqueue kernel" << endl;
	 EXIT_FAIL;
       }
       clFinish(command_queue);
       cout << "done." << endl;
       // read back data
       cl_int berr = clEnqueueReadBuffer (command_queue, kb, CL_TRUE, 0, sizeof (b), &b, 0, NULL, NULL);
       if (berr < 0 ) {
	 cerr << "Failed to read buffer" << endl;
	 EXIT_FAIL;
       }

       
       //  Test the returned data is what its supposed to be.
       cl_bool match = true;
       for ( int i = 0; i < NUM_ITEMS; ++i) {
	 if (4 != b[i] ) {
	   match = false;
	   cout << "Item a[" << i << "]:= " << a[i] 
		<< " == b[" << i << "]:= " << b[i] 
		<< " did not match" << endl;
	 }
       }
       if (!match) {
	 cout << "Not all matched" << endl;
       } else {
	 cout << "All items matched" << endl;
       }
       
       // Clean up objects
       clReleaseMemObject (ka);
       clReleaseMemObject (kb);
       clReleaseCommandQueue (command_queue);
       clReleaseKernel  (kernel);
       clReleaseProgram (program);
       clReleaseContext (context);
      
    }
  }
  
}
