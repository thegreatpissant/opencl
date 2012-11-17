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
cl_int sb_clEnqueueReadBuffer (cl_command_queue command_queue, cl_mem buffer, cl_bool blocking_read, size_t offset, size_t cb, void *ptr, cl_uint num_events_in_wait_list, const cl_event *event_wait_list, cl_event *event);
cl_mem sb_clCreateImage2D (cl_context context, cl_mem_flags flags, const cl_image_format *image_format, size_t image_width, size_t image_height, size_t image_row_pitch, void *host_ptr);
cl_mem sb_clCreateImage3D (cl_context context, cl_mem_flags flags, const cl_image_format *image_format, size_t image_width, size_t image_height, size_t image_row_pitch, void *host_ptr);
cl_int sb_clGetImageInfo (cl_mem image);
void sb_clGetMemObjectInfo (cl_mem object);
#endif
