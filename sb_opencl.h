#include <CL/cl.h>
#include <string>
using std::string;

#ifndef __SB_OPENCL__
#define __SB_OPENCL__

void sb_clPrintPlatformInfo ( cl_platform_id * platform );
void sb_clPrintDeviceInfo ( cl_device_id * device );
int sb_clReadSourceProgramFromDisk ( char * file_name, string * program_buffer);
cl_program sb_clCreateProgramFromSource (string * program_buffer, cl_context context );
cl_int sb_clBuildProgram ( cl_program * program, 
			   int device_num, cl_device_id *devices, 
			   const char * options, 
			   void * callback, void * user_data);
cl_kernel sb_clCreateKernel (cl_program program, const char * function_name);
cl_command_queue sb_clCreateCommandQueue (cl_context context , cl_device_id device, cl_command_queue_properties properties);
cl_int sb_clEnqueueTask ( cl_command_queue command_queue, cl_kernel kernel, cl_uint num_events_in_wait_list, const cl_event *event_wait_list, cl_event *event);
cl_int sb_clSetKernelArg (cl_kernel kernel, cl_uint arg_index, size_t arg_size, const void *arg_value);
cl_mem sb_clCreateBuffer ( cl_context context, cl_mem_flags flags, size_t size, void *host_ptr );
#endif
