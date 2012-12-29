/*
 * James A. Feister openjaf@gmail.com
 * 
 * future soapy brain games OpenCL Helper library
 */ 
#include <CL/cl.h>
#include <string>
using std::string;

#ifndef __SB_OPENCL__
#define __SB_OPENCL__

///  Print cl platform information
void sb_clPrintPlatformInfo ( cl_platform_id * platform );

///  Print a cl device information
void sb_clPrintDeviceInfo ( cl_device_id * device );

/// Read a cl source file from disk and put it in a buffer
int sb_clReadSourceProgramFromDisk ( char * file_name, string * program_buffer);

/// Create a program from a string of the cl program
cl_program sb_clCreateProgramFromSource (string * program_buffer, cl_context context );

/// Build an OpenCL program 
cl_int sb_clBuildProgram ( cl_program * program, 
			   int device_num, cl_device_id *devices, 
			   const char * options, 
			   void * callback, void * user_data);

/// Create a kernel from a function name
cl_kernel sb_clCreateKernel (cl_program program, const char * function_name);

/// Create a command queue
cl_command_queue sb_clCreateCommandQueue (cl_context context , cl_device_id device, cl_command_queue_properties properties);

/// Enqueu a tast
cl_int sb_clEnqueueTask ( cl_command_queue command_queue, cl_kernel kernel, cl_uint num_events_in_wait_list, const cl_event *event_wait_list, cl_event *event);

/// Set kernel arguments
cl_int sb_clSetKernelArg (cl_kernel kernel, cl_uint arg_index, size_t arg_size, const void *arg_value);

/// Create a buffer
cl_mem sb_clCreateBuffer ( cl_context context, cl_mem_flags flags, size_t size, void *host_ptr );

/// Create a Read buffer
cl_int sb_clEnqueueReadBuffer (cl_command_queue command_queue, cl_mem buffer, cl_bool blocking_read, size_t offset, size_t cb, void *ptr, cl_uint num_events_in_wait_list, const cl_event *event_wait_list, cl_event *event);

/// Create a 2d image
cl_mem sb_clCreateImage2D (cl_context context, cl_mem_flags flags, const cl_image_format *image_format, size_t image_width, size_t image_height, size_t image_row_pitch, void *host_ptr);

/// Create a 3d image
cl_mem sb_clCreateImage3D (cl_context context, cl_mem_flags flags, const cl_image_format *image_format, size_t image_width, size_t image_height, size_t image_row_pitch, void *host_ptr);

/// Print information about an image
cl_int sb_clGetImageInfo (cl_mem image);

/// Print information about a cl_mem object
void sb_clGetMemObjectInfo (cl_mem object);

/// Get a device, context, and platform from the first available. 
int sb_clGetContextWithDevice ( cl_platform_id ** platform, cl_context *context, cl_device_id ** device );
#endif
