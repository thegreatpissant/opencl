/*
 * James A. Feister openjaf@gmail.com
 * 
 * future soapy brain games OpenCL Helper library
 */ 
#include <CL/cl.h>
#include <iostream>
#include <cstdlib>
#include <string>
#include <stdio.h>

using std::cout;
using std::cerr;
using std::endl;
using std::string;

//  Should have did this earlier....
#define ERROR_CASE(SWITCH_CASE, ERROR_MESSAGE) \
  case SWITCH_CASE:			       \
  cerr << ERROR_MESSAGE << endl;	       \
  break					       \

#define INFO_CASE(SWITCH_CASE, INFO_MESSAGE) \
  case SWITCH_CASE:			       \
  cout << INFO_MESSAGE << endl;		       \
  break					       \


void sb_clPrintPlatformExtension ( cl_platform_id * platform, cl_int extension );

void sb_clPrintPlatformInfo ( cl_platform_id * platform ) 
{
  cout << "Platform Info\n";

  cout << "Platform Name: ";
  sb_clPrintPlatformExtension ( platform, CL_PLATFORM_NAME );
  cout << endl;

  cout << "Platform Vendor: ";
  sb_clPrintPlatformExtension (  platform, CL_PLATFORM_VENDOR );
  cout << endl;

  cout << "Platform Version: ";
  sb_clPrintPlatformExtension (  platform, CL_PLATFORM_VERSION );
  cout << endl;

  cout << "Platform Profile: ";
  sb_clPrintPlatformExtension (  platform, CL_PLATFORM_PROFILE );
  cout << endl;

  cout << "Platform Extensions: ";
  sb_clPrintPlatformExtension (  platform, CL_PLATFORM_EXTENSIONS );
  cout << endl;
}

void sb_clPrintPlatformExtension ( cl_platform_id * platform, cl_int extension ) 
{
  cl_char * ext_data;
  size_t param_size;
  cl_int ret;

  ret = clGetPlatformInfo (*platform, extension, 0, NULL, &param_size);
  if ( ret < 0 ) {
    cout << "Couldn't read extension data." << endl;
    exit (EXIT_FAILURE);
  }
  ext_data = (cl_char*)malloc(param_size+1);
  ext_data[param_size] = '\0';
  ret = clGetPlatformInfo (*platform, extension, param_size, ext_data, NULL);
  cout <<  ext_data;
  free (ext_data);
}

void sb_clPrintDeviceInfo ( cl_device_id * device )
{
  size_t   qstring_len = 2048;
  cl_char *qstring = (cl_char *) malloc (qstring_len+1);
  cl_device_type qdevice_type;
  size_t qsize_t;
  cl_uint qcl_uint;
  cl_ulong qcl_ulong;
  cl_bool qcl_bool;
  qstring[qstring_len] = '\0';

  clGetDeviceInfo ( *device, CL_DEVICE_VENDOR, qstring_len, qstring, NULL);
  cout << "Device Vendor Name: " << qstring << endl;

  clGetDeviceInfo ( *device, CL_DEVICE_NAME, qstring_len, qstring, NULL);
  cout << "Device Name: " <<  qstring << endl;

  clGetDeviceInfo ( *device, CL_DEVICE_OPENCL_C_VERSION, qstring_len, qstring, NULL);
  cout << "Device OpenCL C version: " <<  qstring << endl;

  clGetDeviceInfo ( *device, CL_DRIVER_VERSION, qstring_len, qstring, NULL);
  cout << "Device OpenCL Driver version: " <<  qstring << endl;
 
  clGetDeviceInfo ( *device, CL_DEVICE_TYPE, sizeof (cl_device_type), &qdevice_type, NULL);
  cout << "Device type is: ";
  switch (qdevice_type) {
    INFO_CASE ( CL_DEVICE_TYPE_CPU,
		"CPU" );
    INFO_CASE ( CL_DEVICE_TYPE_GPU,
		"GPU" );
    INFO_CASE ( CL_DEVICE_TYPE_ACCELERATOR,
		"ACCELERATOR" );
    INFO_CASE ( CL_DEVICE_TYPE_DEFAULT,
		"DEFAULT" );
  default: 
    cout << "Unknow device" << endl;
    break;
  }
 
  clGetDeviceInfo ( *device, CL_DEVICE_EXTENSIONS, qstring_len, qstring, NULL);
  cout << "Device Extensions: " << qstring << endl;

  clGetDeviceInfo ( *device, CL_DEVICE_IMAGE_SUPPORT, sizeof (cl_bool), &qcl_bool, NULL);
  cout << "Device supports images: " << (qcl_bool ? "TRUE":"FALSE") << endl;

  clGetDeviceInfo ( *device, CL_DEVICE_MAX_COMPUTE_UNITS, sizeof (cl_uint), &qcl_uint, NULL);
  cout << "Device max compute units: " << qcl_uint << endl;

  clGetDeviceInfo ( *device, CL_DEVICE_MAX_WORK_ITEM_DIMENSIONS, sizeof (cl_uint), &qcl_uint, NULL);
  cout << "Device max work item dimensions: " << qcl_uint << endl;

  clGetDeviceInfo ( *device, CL_DEVICE_MAX_WORK_GROUP_SIZE, sizeof (size_t), &qsize_t, NULL);
  cout << "Device max work group size: " << qsize_t << endl;

  clGetDeviceInfo ( *device, CL_DEVICE_MAX_WORK_ITEM_SIZES, sizeof (cl_uint), &qcl_uint, NULL);
  cout << "Device max work item sizes: " << qcl_uint << endl;

  clGetDeviceInfo ( *device, CL_DEVICE_GLOBAL_MEM_SIZE, sizeof (cl_ulong), &qcl_ulong, NULL);
  cout << "Device Global Memory Size: " << qcl_ulong << endl;

  clGetDeviceInfo ( *device, CL_DEVICE_GLOBAL_MEM_CACHE_SIZE, sizeof (cl_ulong), &qcl_ulong, NULL);
  cout << "Device Global Memory Cache Size: " << qcl_ulong << endl;
  
  clGetDeviceInfo ( *device, CL_DEVICE_MAX_CONSTANT_BUFFER_SIZE, sizeof (cl_ulong), &qcl_ulong, NULL);
  cout << "Device Max Constant Buffer Size: " << qcl_ulong << endl;
  
  clGetDeviceInfo ( *device, CL_DEVICE_LOCAL_MEM_SIZE, sizeof (cl_ulong), &qcl_ulong, NULL);
  cout << "Device Local Memory Size: " << qcl_ulong << endl;

  clGetDeviceInfo ( *device, CL_DEVICE_ADDRESS_BITS, sizeof (cl_uint), &qcl_uint, NULL);
  cout << "Device Address Bits: " << qcl_uint << endl;

  clGetDeviceInfo ( *device, CL_DEVICE_AVAILABLE, sizeof (cl_bool), &qcl_bool, NULL);
  cout << "Device Available: " << (qcl_bool ? "TRUE":"FALSE" ) << endl;

  clGetDeviceInfo ( *device, CL_DEVICE_COMPILER_AVAILABLE, sizeof (cl_bool), &qcl_bool, NULL);
  cout << "Device Compiler: " << (qcl_bool ? "TRUE":"FALSE") << endl;

  
  cout << "Preferred Vector width's." << endl;
  clGetDeviceInfo (*device, CL_DEVICE_PREFERRED_VECTOR_WIDTH_CHAR,
		   sizeof (cl_uint), &qcl_uint, NULL);
  cout << "\tIn chars: " << qcl_uint << endl;
  clGetDeviceInfo (*device, CL_DEVICE_PREFERRED_VECTOR_WIDTH_SHORT,
		   sizeof (cl_uint), &qcl_uint, NULL);
  cout << "\tIn shorts: " << qcl_uint << endl;
  clGetDeviceInfo (*device, CL_DEVICE_PREFERRED_VECTOR_WIDTH_INT,
		   sizeof (cl_uint), &qcl_uint, NULL);
  cout << "\tIn ints: " << qcl_uint << endl;
  clGetDeviceInfo (*device, CL_DEVICE_PREFERRED_VECTOR_WIDTH_LONG,
		   sizeof (cl_uint), &qcl_uint, NULL);
  cout << "\tIn longs: " << qcl_uint << endl;
  clGetDeviceInfo (*device, CL_DEVICE_PREFERRED_VECTOR_WIDTH_FLOAT,
		   sizeof (cl_uint), &qcl_uint, NULL);
  cout << "\tIn floats: " << qcl_uint << endl;
  clGetDeviceInfo (*device, CL_DEVICE_PREFERRED_VECTOR_WIDTH_DOUBLE,
		   sizeof (cl_uint), &qcl_uint, NULL);
  cout << "\tIn doubles: " << qcl_uint << endl;

#ifdef CL_DEVICE_PREFERRED_VECTOR_WIDTH_HALF
  clGetDeviceInfo (*device, CL_DEVICE_PREFERRED_VECTOR_WIDTH_HALF,
    sizeof (cl_uint), &qcl_uint, NULL);
  cout << "\tIn halfs: " << qcl_uint << endl;
#endif
  cl_device_fp_config flag;
  clGetDeviceInfo (*device, CL_DEVICE_SINGLE_FP_CONFIG,
		   sizeof (flag), &flag, NULL);
  cout << "Device Floating processing features: " << endl;
  switch (flag) {
    INFO_CASE ( CL_FP_INF_NAN,
		"\tINF and NaN values supported.");
    INFO_CASE ( CL_FP_DENORM,
		"\tDenormalized numbers supported.");
    INFO_CASE ( CL_FP_ROUND_TO_NEAREST,
		"\tRound to Nearest Even mode supported." );
    INFO_CASE ( CL_FP_ROUND_TO_INF,
		"\tRound to Infinity mode supported." );
    INFO_CASE ( CL_FP_ROUND_TO_ZERO,
		"\tRound to Zero mode supported." );
    INFO_CASE ( CL_FP_FMA,
		"\tFloating-point multiply-and-add operation supported" );
  default: 
    cout << "Unknown floating point processing feature" << endl;
    break;
  }
}

int sb_clReadSourceProgramFromDisk ( char * file_name, string * program_buffer )
{
  size_t file_size;
  FILE *file_handle;
  char * buffer;

  file_handle = fopen (file_name, "r");
  if ( NULL == file_handle ) {
    perror ("Error opening file\n"); 
    return -1;
  }

  fseek (file_handle, 0, SEEK_END);
  file_size = ftell (file_handle);
  rewind (file_handle);
  
  buffer = (char *)malloc (sizeof (char)*(file_size + 1));
  buffer[file_size] = '\0';
  fread ( buffer , sizeof (char), file_size, file_handle);
  fclose (file_handle);
  *program_buffer = buffer;
  delete (buffer);
  //  cout << "Program: " << *program_buffer << endl;
  return program_buffer->size();
}

cl_program sb_clCreateProgramFromSource ( string * program_buffer, cl_context context )
{
  cl_program program;
  cl_int error_code;
  
  char * source = (char *) malloc (sizeof (char ) * (program_buffer->length() + 1));
  source[program_buffer->length()] = '\0';
  program_buffer->copy(source, program_buffer->length());
  program = clCreateProgramWithSource (context, 1, (const char **) &source, NULL, &error_code);
  if ( program == NULL || error_code < 0 ) {
    switch (error_code) {
      ERROR_CASE (CL_INVALID_CONTEXT,
		  "Source Program: Invalid Context");
      ERROR_CASE (CL_INVALID_VALUE,
		  "Source Program: Invalid Value");
      ERROR_CASE (CL_OUT_OF_HOST_MEMORY,
		  "Source Program: Out of host memory");
    default:
      cerr << "Source Program: Unknown error\n";
      break;
    }
    return NULL;
  }

  return program;
} 

cl_int sb_clBuildProgram ( cl_program * program, 
			   int device_num, cl_device_id *devices, 
			   const char * options, 
			   void * callback, void * user_data)
{

  cl_int program_ret;

  program_ret = clBuildProgram (*program, 1, devices, options, (void(*)(cl_program, void*))callback, user_data);
  
  switch (program_ret) {
  case CL_SUCCESS:
    cout << "Program built without errors." << endl;
    break;
    ERROR_CASE (CL_INVALID_PROGRAM,
		"Error: Invalid Program Object.");
    break;
    ERROR_CASE (CL_INVALID_VALUE,
		"Error: Invalid Value for devices.");
    ERROR_CASE (CL_INVALID_DEVICE,
		"Error: Invalid device list.");
    ERROR_CASE (CL_INVALID_BINARY,
		"Error: Ivalid Binary for devices.");
    ERROR_CASE (CL_INVALID_BUILD_OPTIONS,
		"Error: Invalid build options.");
    //  @@TODO: Why is this a duplicate define?
#ifndef  CL_INVALID_OPERATION
      case CL_INVALID_OPERATION:
        cerr << "Error: Program build did not complete." << endl;
        break;
#endif
    ERROR_CASE (CL_COMPILER_NOT_AVAILABLE,
		"Error: Source compilation not available for device.");
    ERROR_CASE (CL_BUILD_PROGRAM_FAILURE,
		"Error: Failure to build program.");
    ERROR_CASE (CL_INVALID_OPERATION,
		"Error: Kernel objects already attached to program.");
    ERROR_CASE (CL_OUT_OF_RESOURCES,
		"Error: Failure to allocate required resources.");
    ERROR_CASE (CL_OUT_OF_HOST_MEMORY,
		"Error: Failure to allocate memory resources.");
  default:
    cerr << "Error: Unknown error building Program." << endl;
    break;
  }
  if (program_ret != CL_SUCCESS) {
    size_t build_param_size;
    char * build_param_value;
    cl_int build_ret = clGetProgramBuildInfo (*program, devices[0],
			   CL_PROGRAM_BUILD_LOG,
			   1, NULL,
			   &build_param_size);
    switch (build_ret) {
      ERROR_CASE (CL_INVALID_DEVICE,
		  "Getting Build info failed: Invalid Device");
      ERROR_CASE (CL_INVALID_VALUE,
		  "Getting Build info failed: Invalid Value");
      ERROR_CASE (CL_INVALID_PROGRAM,
		  "Getting Build info failed: Invalid Program");
      ERROR_CASE ( CL_OUT_OF_RESOURCES,
		   "Getting Build info failed: OUT OF RESOURCES");
      ERROR_CASE ( CL_OUT_OF_HOST_MEMORY,
		   "Getting Build info failed: OUT OF HOST MEMORY");
    }
    build_param_value = (char *) malloc (sizeof (char)*(build_param_size+1));
    build_param_value[build_param_size] = '\0';
    cl_int build_status = clGetProgramBuildInfo (*program, devices[0],
						 CL_PROGRAM_BUILD_LOG,
						 build_param_size, build_param_value,
						 NULL);

    if (build_status != CL_SUCCESS) {
      cerr << "Error: obtaining build status failed." << endl;
      return -1;
    }
    cout << "Compiler log: " << endl
	 << build_param_value << endl;
    return -1;
  }
  return program_ret;
}

cl_kernel sb_clCreateKernel (cl_program program, const char * function_name)
{
  cl_kernel kernel;
  cl_int kern_error;

  kernel = clCreateKernel (program, function_name, &kern_error);
  if (kernel == NULL) {
    switch(kern_error) {
      ERROR_CASE (CL_INVALID_PROGRAM,
		  "Program is not a valid program object.");
      ERROR_CASE (CL_INVALID_PROGRAM_EXECUTABLE,
		  "There is no successfully built executable for program.");
      ERROR_CASE (CL_INVALID_KERNEL_NAME,
		   "kernel_name \"" << function_name << "\" is not found in program.");
      ERROR_CASE (CL_INVALID_KERNEL_DEFINITION,
		  "The function definition for __kernel function given by kernel_name such as the number of arguments, the argument types are not the same for all devices for which the program executable has been built.");
      ERROR_CASE (CL_INVALID_VALUE,
		  "kernel_name is NULL.");
      ERROR_CASE (CL_OUT_OF_RESOURCES,
		  "Failure to allocate resources required by the OpenCL implementation on the device.");
      ERROR_CASE (CL_OUT_OF_HOST_MEMORY,
		  "Failure to allocate resources required by the OpenCL implementation on the host.");
    default:
      cerr << "Unknown error." << endl;
      break;
    }
  }
  return kernel;
}

cl_command_queue sb_clCreateCommandQueue (cl_context context , cl_device_id device, cl_command_queue_properties properties)
{
  cl_command_queue command_queue;
  cl_int queue_error;

  command_queue = clCreateCommandQueue (context, device, properties, &queue_error);
  if (command_queue == NULL) {
    switch (queue_error) {
      ERROR_CASE (CL_INVALID_CONTEXT,
		  "Context is not a valid context.");
      ERROR_CASE (CL_INVALID_DEVICE,
		  "Device is not a valid device or is not associated with context.");
      ERROR_CASE (CL_INVALID_VALUE,
		  "Values specified in properties are not valid.");
      ERROR_CASE (CL_INVALID_QUEUE_PROPERTIES,
		  "Values specified in properties are valid but are not supported by the device.");
      ERROR_CASE  (CL_OUT_OF_RESOURCES,
		   "Failure to allocate resources required by the OpenCL implementation on the device.");
      ERROR_CASE (CL_OUT_OF_HOST_MEMORY,
		  "Failure to allocate resources required by the OpenCL implementation on the host.");
    default:
      cerr << "Unknown error." << endl;;
      break;
    }
  }
  return command_queue;
}

cl_int sb_clEnqueueTask ( cl_command_queue command_queue, cl_kernel kernel, cl_uint num_events_in_wait_list, const cl_event *event_wait_list, cl_event *event)
{
  cl_int task_error;
  task_error = clEnqueueTask (command_queue, kernel, num_events_in_wait_list, event_wait_list, event);
  if (task_error != CL_SUCCESS) {
    cerr << "ERROR clEnqueueTask: ";
    switch ( task_error) {
      ERROR_CASE (CL_INVALID_PROGRAM_EXECUTABLE,
		  "There is no successfully built program executable available for device associated with command_queue. ");

      ERROR_CASE (CL_INVALID_COMMAND_QUEUE,
		  "Command_queue is not a valid command-queue.");
      ERROR_CASE (CL_INVALID_KERNEL,
		  "Kernel is not a valid kernel object.");
      ERROR_CASE (CL_INVALID_CONTEXT,
		  "Context associated with command_queue and kernel is not the same or if the context associated with command_queue and events in event_wait_list are not the same.");
      ERROR_CASE (CL_INVALID_KERNEL_ARGS,
		  "The kernel argument values have not been specified.");
      ERROR_CASE (CL_INVALID_WORK_GROUP_SIZE,
		  "A work-group size is specified for kernel using the __attribute__((reqd_work_group_size(X, Y, Z))) qualifier in program source and is not (1, 1, 1).");
#ifdef CL_MISALIGNED_SUB_BUFFER_OFFSET
	  case CL_MISALIGNED_SUB_BUFFER_OFFSET: 
	  cerr << "A sub-buffer object is specified as the value for an argument that is a buffer object and the offset specified when the sub-buffer object is created is not aligned to CL_DEVICE_MEM_BASE_ADDR_ALIGN value for device associated with queue." << endl;
	  break;
#endif
      ERROR_CASE (CL_INVALID_IMAGE_SIZE,
		  "Image object is specified as an argument value and the image dimensions (image width, height, specified or compute row and/or slice pitch) are not supported by device associated with queue.");
      ERROR_CASE (CL_OUT_OF_RESOURCES,
		  "There is a failure to queue the execution instance of kernel on the command-queue because of insufficient resources needed to execute the kernel.");
      ERROR_CASE (CL_MEM_OBJECT_ALLOCATION_FAILURE,
		  "There is a failure to allocate memory for data store associated with image or buffer objects specified as arguments to kernel.");
      ERROR_CASE (CL_INVALID_EVENT_WAIT_LIST,
		   "Event_wait_list is NULL and num_events_in_wait_list is greater than 0, or event_wait_list is not NULL and num_events_in_wait_list is 0, or if event objects in event_wait_list are not valid events.");
      ERROR_CASE (CL_OUT_OF_HOST_MEMORY,
		  "There is a failure to allocate resources required by the OpenCL implementation on the host.");
    default:
      cerr << "Unknown error occured" << endl;
      break;
    }
  }
  return task_error;
}

cl_int sb_clSetKernelArg (cl_kernel kernel, cl_uint arg_index, size_t arg_size, const void *arg_value)
{
  cl_int arg_error;
  arg_error = clSetKernelArg( kernel, arg_index, arg_size, arg_value);
  if (arg_error != CL_SUCCESS) {
    cerr << "ERROR clSetKernelArg: ";
    switch (arg_error) {
      ERROR_CASE (CL_INVALID_KERNEL,
		  "Kernel is not a valid kernel object.");
      ERROR_CASE (CL_INVALID_ARG_INDEX,
		  "arg_index is not a valid argument index.");
      ERROR_CASE (CL_INVALID_ARG_VALUE,
		  "arg_value specified is NULL for an argument that is not declared "
		  "with the __local qualifier or vice-versa.");
      ERROR_CASE (CL_INVALID_MEM_OBJECT,
		  "An argument declared to be a memory object when the specified arg_value "
		  "is not a valid memory object.");
      ERROR_CASE (CL_INVALID_SAMPLER, 
		  "An argument declared to be of type sampler_t when the specified arg_value "
		  "is not a valid sampler object.");
      ERROR_CASE (CL_INVALID_ARG_SIZE, "arg_size does not match the size of the data type for "
		  "an argument that is not a memory object or if the argument is a memory "
		  "object and arg_size != sizeof(cl_mem) or if arg_size is zero and the argument "
		  "is declared with the __local qualifier or if the argument is a sampler and "
		  "arg_size != sizeof(cl_sampler).");
      ERROR_CASE (CL_OUT_OF_RESOURCES, 
		  "There is a failure to allocate resources required by the OpenCL implementation "
		  "on the device.");
      ERROR_CASE (CL_OUT_OF_HOST_MEMORY,
		  "There is a failure to allocate resources required by the OpenCL implementation "
		  "on the host.");
    default:
      cerr << "Unknown error" << endl;
      break;
    }
  }
  return arg_error;
}
cl_mem sb_clCreateBuffer ( cl_context context, cl_mem_flags flags, size_t size, void *host_ptr )
{
  cl_int buffer_error_code;
  cl_mem mem_object;

  mem_object = clCreateBuffer (context, flags, size, host_ptr, &buffer_error_code);
  if (mem_object == NULL) {
    cerr << "Failed to wrap kernel arg in memory buffer object " << endl;
    switch (buffer_error_code) {
      ERROR_CASE (CL_INVALID_CONTEXT,
		  "if context is not a valid context..");
      ERROR_CASE (CL_INVALID_VALUE,
		  "if values specified in flags are not valid.");
      ERROR_CASE (CL_INVALID_BUFFER_SIZE, 
		  "if size is 0 or is greater than CL_DEVICE_MAX_MEM_ALLOC_SIZE value specified in table of OpenCL Device Queries for clGetDeviceInfo for all devices in context.");
      ERROR_CASE (CL_INVALID_HOST_PTR,
		  "if host_ptr is NULL and CL_MEM_USE_HOST_PTR or CL_MEM_COPY_HOST_PTR are set in flags or if host_ptr is not NULL but CL_MEM_COPY_HOST_PTR or CL_MEM_USE_HOST_PTR are not set in flags.");
      ERROR_CASE (CL_MEM_OBJECT_ALLOCATION_FAILURE,
		  "if there is a failure to allocate memory for buffer object.");
      ERROR_CASE (CL_OUT_OF_HOST_MEMORY,
		  "if there is a failure to allocate resources required by the OpenCL implementation on the host.");
    default:
      cerr << "Unknown error" << endl;
      break;
    }
  }
  return mem_object;
}

cl_int sb_clEnqueueReadBuffer (cl_command_queue command_queue,
			       cl_mem buffer,
			       cl_bool blocking_read,
			       size_t offset,
			       size_t cb,
			       void *ptr,
			       cl_uint num_events_in_wait_list,
			       const cl_event *event_wait_list,
			       cl_event *event)
{

  cl_int read_buffer_ret;
  read_buffer_ret = clEnqueueReadBuffer (command_queue, buffer, blocking_read, offset, cb, ptr,
		       num_events_in_wait_list, event_wait_list, event);

  if (read_buffer_ret != CL_SUCCESS) {
    cerr << "Error Enqueuing buffer to read. " << endl;

    switch (read_buffer_ret) {
      
      ERROR_CASE (CL_INVALID_COMMAND_QUEUE,
		 "if command_queue is not a valid command-queue.");
      ERROR_CASE (CL_INVALID_CONTEXT,
		 "if the context associated with command_queue and buffer are not the same or if the context associated with command_queue and events in event_wait_list are not the same.");
      ERROR_CASE (CL_INVALID_MEM_OBJECT,
		 "if buffer is not a valid buffer object.");
      ERROR_CASE (CL_INVALID_VALUE,
		 "if the region being read specified by (offset, cb) is out of bounds or if ptr is a NULL value.");
      ERROR_CASE (CL_INVALID_EVENT_WAIT_LIST,
		 "if event_wait_list is NULL and num_events_in_wait_list greater than 0, or event_wait_list is not NULL and num_events_in_wait_list is 0, or if event objects in event_wait_list are not valid events.");
      ERROR_CASE (CL_MISALIGNED_SUB_BUFFER_OFFSET,
		 "if buffer is a sub-buffer object and offset specified when the sub-buffer object is created is not aligned to CL_DEVICE_MEM_BASE_ADDR_ALIGN value for device associated with queue.");
      ERROR_CASE (CL_EXEC_STATUS_ERROR_FOR_EVENTS_IN_WAIT_LIST,
		 "if the read and write operations are blocking and the execution status of any of the events in event_wait_list is a negative integer value.");
      ERROR_CASE (CL_MEM_OBJECT_ALLOCATION_FAILURE,
		 "if there is a failure to allocate memory for data store associated with buffer.");
      ERROR_CASE (CL_OUT_OF_RESOURCES,
		 "if there is a failure to allocate resources required by the OpenCL implementation on the device.");
      ERROR_CASE (CL_OUT_OF_HOST_MEMORY,
		 "if there is a failure to allocate resources required by the OpenCL implementation on the host.");
    default:
      cerr << "Unknown error occured" << endl;
      break;
    }
  }

  return read_buffer_ret;
}

cl_mem sb_clCreateImage2D (cl_context context, cl_mem_flags flags,
			   const cl_image_format *image_format,
			   size_t image_width, size_t image_height,
			   size_t image_row_pitch,
			   void *host_ptr)
{

  cl_int error_2d;
  cl_mem mem_object;

  mem_object = clCreateImage2D (context, flags, image_format, image_width, image_height,
				image_row_pitch, host_ptr, &error_2d);

  if (mem_object == NULL) {
    cerr << "Error createing 2d image memory object" << endl;
    switch (error_2d) {
      ERROR_CASE (CL_INVALID_CONTEXT,
		  "if context is not a valid context.");
      ERROR_CASE (CL_INVALID_VALUE,
		  "if values specified in flags are not valid.");
      ERROR_CASE (CL_INVALID_IMAGE_FORMAT_DESCRIPTOR,
		  "if values specified in image_format are not valid or if image_format is NULL.");
      ERROR_CASE (CL_INVALID_IMAGE_SIZE,
		  " if image_width or image_height are 0 or if they exceed values specified in CL_DEVICE_IMAGE2D_MAX_WIDTH or CL_DEVICE_IMAGE2D_MAX_HEIGHT respectively for all devices in context or if values specified by image_row_pitch do not follow rules described in the argument description above.");
      ERROR_CASE (CL_INVALID_HOST_PTR,
		  "if host_ptr is NULL and CL_MEM_USE_HOST_PTR or CL_MEM_COPY_HOST_PTR are set in flags or if host_ptr is not NULL but CL_MEM_COPY_HOST_PTR or CL_MEM_USE_HOST_PTR are not set in flags.");
      ERROR_CASE (CL_IMAGE_FORMAT_NOT_SUPPORTED,
		  "if the image_format is not supported.");
      ERROR_CASE (CL_MEM_OBJECT_ALLOCATION_FAILURE,
		  " if there is a failure to allocate memory for image object.");
      ERROR_CASE (CL_INVALID_OPERATION,
		  "if there are no devices in context that support images (i.e. CL_DEVICE_IMAGE_SUPPORT (specified in the table of OpenCL Device Queries for clGetDeviceInfo) is CL_FALSE).");
      ERROR_CASE (CL_OUT_OF_RESOURCES,
		  "if there is a failure to allocate resources required by the OpenCL implementation on the device.");
      ERROR_CASE (CL_OUT_OF_HOST_MEMORY,
		  "if there is a failure to allocate resources required by the OpenCL implementation on the host.");
    default:
      cerr << "Unknown error" << endl;
      break;
    }
  }
  return mem_object;
}

cl_mem sb_clCreateImage2D (cl_context context,
			   cl_mem_flags flags,
			   const cl_image_format *image_format,
			   size_t image_width, size_t image_height, size_t image_depth,
			   size_t image_row_pitch, size_t image_slice_pitch,
			   void *host_ptr)
{

  cl_int error_3d;
  cl_mem mem_object;

  mem_object = clCreateImage3D (context, flags, image_format, 
				image_width, image_height, image_depth,
				image_row_pitch, image_slice_pitch, host_ptr, &error_3d);

  if (mem_object == NULL) {
    cerr << "Error createing 3d image memory object" << endl;
    switch (error_3d) {
      ERROR_CASE (CL_INVALID_CONTEXT,
		  "if context is not a valid context.");
      ERROR_CASE (CL_INVALID_VALUE,
		  "if values specified in flags are not valid.");
      ERROR_CASE (CL_INVALID_IMAGE_FORMAT_DESCRIPTOR,
		  "if values specified in image_format are not valid or if image_format is NULL.");
      ERROR_CASE (CL_INVALID_IMAGE_SIZE,
		  " if image_width or image_height are 0 or if they exceed values specified in CL_DEVICE_IMAGE2D_MAX_WIDTH or CL_DEVICE_IMAGE2D_MAX_HEIGHT respectively for all devices in context or if values specified by image_row_pitch do not follow rules described in the argument description above.");
      ERROR_CASE (CL_INVALID_HOST_PTR,
		  "if host_ptr is NULL and CL_MEM_USE_HOST_PTR or CL_MEM_COPY_HOST_PTR are set in flags or if host_ptr is not NULL but CL_MEM_COPY_HOST_PTR or CL_MEM_USE_HOST_PTR are not set in flags.");
      ERROR_CASE (CL_IMAGE_FORMAT_NOT_SUPPORTED,
		  "if the image_format is not supported.");
      ERROR_CASE (CL_MEM_OBJECT_ALLOCATION_FAILURE,
		  " if there is a failure to allocate memory for image object.");
      ERROR_CASE (CL_INVALID_OPERATION,
		  "if there are no devices in context that support images (i.e. CL_DEVICE_IMAGE_SUPPORT (specified in the table of OpenCL Device Queries for clGetDeviceInfo) is CL_FALSE).");
      ERROR_CASE (CL_OUT_OF_RESOURCES,
		  "if there is a failure to allocate resources required by the OpenCL implementation on the device.");
      ERROR_CASE (CL_OUT_OF_HOST_MEMORY,
		  "if there is a failure to allocate resources required by the OpenCL implementation on the host.");
    default:
      cerr << "Unknown error" << endl;
      break;
    }
  }
  return mem_object;
}

static void sb_print_clGetInfoError (cl_int image_info_error);
static void sb_print_cl_image_format (cl_image_format *image_format);

cl_int sb_clGetImageInfo (cl_mem image)
{
  cl_int info_ret;

  //  CL_IMAGE_FORMAT
  cl_image_format image_format;
  info_ret = clGetImageInfo (image, CL_IMAGE_FORMAT, 
			     sizeof (cl_image_format), &image_format, NULL);
  if (info_ret != CL_SUCCESS) 
    sb_print_clGetInfoError (info_ret);
  else
    sb_print_cl_image_format (&image_format);
  
  //  CL_IMAGE_ELEMENT_SIZE
  size_t element_size;
  info_ret = clGetImageInfo (image, CL_IMAGE_ELEMENT_SIZE,
			     sizeof (size_t), &element_size, NULL);
  if (info_ret != CL_SUCCESS)
    sb_print_clGetInfoError (info_ret);
  else
    cout << "Size of each image element: " << element_size << endl;

  //  CL_IMAGE_ROW_PITCH
  info_ret = clGetImageInfo (image, CL_IMAGE_ROW_PITCH,
			     sizeof (size_t), &element_size, NULL);
  if (info_ret != CL_SUCCESS)
    sb_print_clGetInfoError (info_ret);
  else
    cout << "Size of row elements: " << element_size << endl;

  //  CL_IMAGE_SLICE_PITCH
  info_ret = clGetImageInfo (image, CL_IMAGE_SLICE_PITCH,
			     sizeof (size_t), &element_size, NULL);
  if (info_ret != CL_SUCCESS)
    sb_print_clGetInfoError (info_ret);
  else
    cout << "Size of 2D slice in 3D image: " << element_size << endl; 

  //  CL_IMAGE_WIDTH
  info_ret = clGetImageInfo (image, CL_IMAGE_WIDTH,
			     sizeof (size_t), &element_size, NULL);
  if (info_ret != CL_SUCCESS)
    sb_print_clGetInfoError (info_ret);
  else
    cout << "Width of image pixels: " << element_size << endl;

  //  CL_IMAGE_HEIGHT
  info_ret = clGetImageInfo (image, CL_IMAGE_HEIGHT,
			     sizeof (size_t), &element_size, NULL);
  if (info_ret != CL_SUCCESS)
    sb_print_clGetInfoError (info_ret);
  else
    cout << "Height of image in pixels: " << element_size << endl;

  //  CL_IMAGE_DEPTH
  info_ret = clGetImageInfo (image, CL_IMAGE_DEPTH,
			     sizeof (size_t), &element_size, NULL);
  if (info_ret != CL_SUCCESS)
    sb_print_clGetInfoError (info_ret);
  else
    cout << "Depth of image in pixels: " << element_size << endl;

}

static void sb_print_cl_image_format (cl_image_format *image_format) 
{
  cout << "Image format " << endl;
  cout << "channel_order " << endl;
  switch (image_format->image_channel_order) {
  case CL_R:
  case CL_Rx:
  case CL_A:
    cout << "CL_R, CL_Rx, CL_A " << endl;
    break;
  case CL_INTENSITY:
    cout << "CL_INTENSITY" << endl;
    break;
  case CL_LUMINANCE :
    cout << "CL_LUMINANCE" << endl;
    break;
  case CL_RG:
  case CL_RGx:
  case CL_RA:
    cout << "CL_RG, CL_RGx, CL_RA" << endl;
    break;
  case CL_RGB:
  case CL_RGBx:
    cout << "CL_RGB, CL_RGBx" << endl;
    break;
  case CL_RGBA:
    cout << "CL_RGBA" << endl;
    break;
  case CL_ARGB:
  case CL_BGRA:
    cout << "CL_ARGB, CL_BRGA" << endl;
    break;
  default:
    break;
  }

  cout << "size of channel data type" << endl;

  switch (image_format->image_channel_data_type) {
    INFO_CASE (CL_SNORM_INT8,
	       "Each channel component is a normalized signed 8-bit integer value.");
    INFO_CASE (CL_SNORM_INT16,
	       "Each channel component is a normalized signed 16-bit integer value.");
    INFO_CASE (CL_UNORM_INT8,
	       "Each channel component is a normalized unsigned 8-bit integer value.");
    INFO_CASE (CL_UNORM_INT16,
	       "Each channel component is a normalized unsigned 16-bit integer value.");
    INFO_CASE (CL_UNORM_SHORT_565,
	       "Represents a normalized 5-6-5 3-channel RGB image. The channel order must be CL_RGB.");
    INFO_CASE (CL_UNORM_SHORT_555,
	       "Represents a normalized x-5-5-5 4-channel xRGB image. The channel order must be CL_RGB.");
    INFO_CASE (CL_UNORM_INT_101010,
	       "Represents a normalized x-10-10-10 4-channel xRGB image. The channel order must be CL_RGB.");
    INFO_CASE (CL_SIGNED_INT8,
	       "Each channel component is an unnormalized signed 8-bit integer value.");
    INFO_CASE (CL_SIGNED_INT16,
	       "Each channel component is an unnormalized signed 16-bit integer value.");
    INFO_CASE (CL_SIGNED_INT32,
	       "Each channel component is an unnormalized signed 32-bit integer value.");
    INFO_CASE (CL_UNSIGNED_INT8,
	       "Each channel component is an unnormalized unsigned 8-bit integer value.");
    INFO_CASE (CL_UNSIGNED_INT16,
	       "Each channel component is an unnormalized unsigned 16-bit integer value.");
    INFO_CASE (CL_UNSIGNED_INT32,
	       "Each channel component is an unnormalized unsigned 32-bit integer value.");
    INFO_CASE (CL_HALF_FLOAT,
	       "Each channel component is a 16-bit half-float value.");
    INFO_CASE (CL_FLOAT,
	       "Each channel component is a single precision floating-point value.");
  default:
    break;
  }

}
static void sb_print_clGetInfoError (cl_int image_info_error) 
{
  switch (image_info_error) {
    ERROR_CASE (CL_INVALID_MEM_OBJECT,
		"if is a not a valid object.");
    ERROR_CASE (CL_INVALID_VALUE,
		"if param_name is not valid, or if size in bytes specified by param_value_size is less than the size of return type as described in the table above and param_value is not NULL.");
#ifdef CL_INVALID_D3D10_RESOURCE_KHR
      ERROR_CASE (CL_INVALID_D3D10_RESOURCE_KHR,
		"if param_name is CL_MEM_D3D10_SUBRESOURCE_KHR and image was not created by the function clCreateFromD3D10Texture2DKHR or clCreateFromD3D10Texture3DKHR.");
#endif
    ERROR_CASE (CL_OUT_OF_RESOURCES,
		"if there is a failure to allocate resources required by the OpenCL implementation on the device.");
    ERROR_CASE (CL_OUT_OF_HOST_MEMORY,
		"if there is a failure to allocate resources required by the OpenCL implementation on the host.");
  default:
    cerr << "Unknown error" << endl;
    break;
  }
}

void sb_clGetMemObjectInfo (cl_mem object)
{
  
  cl_int info_ret;

  cl_mem_object_type obj_type;

  info_ret = clGetMemObjectInfo (object, CL_MEM_TYPE, sizeof (cl_mem_object_type), &obj_type, NULL);
  if (info_ret != CL_SUCCESS)
    sb_print_clGetInfoError (info_ret);
  else				 
    switch (obj_type) {
      INFO_CASE (CL_MEM_OBJECT_BUFFER, "CL_MEM_OBJECT_BUFFER");
      INFO_CASE (CL_MEM_OBJECT_IMAGE2D, "CL_MEM_OBJECT_IMAGE2D");
      INFO_CASE (CL_MEM_OBJECT_IMAGE3D, "CL_MEM_OBJECT_IMAGE3D");
    default: cout << "Unknown object type" << endl;
      break;
    }

  cl_mem_flags mem_flags;
  info_ret = clGetMemObjectInfo (object, CL_MEM_FLAGS, sizeof (cl_mem_flags), &mem_flags, NULL);
  if (info_ret != CL_SUCCESS)
    sb_print_clGetInfoError (info_ret);
  else
    cout << "MemFlags: " << mem_flags << endl;

  void * host_pointer;
  info_ret = clGetMemObjectInfo (object, CL_MEM_HOST_PTR, sizeof (void*), &host_pointer, NULL);
  if (info_ret != CL_SUCCESS)
    sb_print_clGetInfoError (info_ret);
  else
    cout << "Host pointer: " << host_pointer << endl;
  
  size_t mem_size;
  info_ret = clGetMemObjectInfo (object, CL_MEM_SIZE, sizeof (size_t), &mem_size, NULL);
  if (info_ret != CL_SUCCESS)
    sb_print_clGetInfoError (info_ret);
  else
    cout << "Mem size: " << mem_size << endl;
  
  cl_context context;
  info_ret = clGetMemObjectInfo (object, CL_MEM_CONTEXT, sizeof (cl_context), &context, NULL);
  if (info_ret != CL_SUCCESS)
    sb_print_clGetInfoError (info_ret);
  else
    cout << "Mem Context: " << context << endl;

  size_t mem_offset;
  info_ret = clGetMemObjectInfo (object, CL_MEM_OFFSET, sizeof (size_t), &mem_offset, NULL);
  if (info_ret != CL_SUCCESS)
    sb_print_clGetInfoError (info_ret);
  else
    cout << "Mem offset: " << mem_offset << endl;  

  cl_uint ref_count;
  info_ret = clGetMemObjectInfo (object, CL_MEM_REFERENCE_COUNT, sizeof (cl_uint), &ref_count, NULL);
  if (info_ret != CL_SUCCESS)
    sb_print_clGetInfoError (info_ret);
  else
    cout << "Memory objects Reference count: " << ref_count << endl;  

#ifdef CL_MEM_D3D10_RESOURCE_KHR
  ID3D10Resource * d3dresource;
  info_ret = clGetMemObjectInfo (object,  CL_MEM_D3D10_RESOURCE_KHR, sizeof (ID3D10Resource), &d3dresource, NULL);
  if (info_ret != CL_SUCCESS)
    sb_print_clGetInfoError (info_ret);
  else
    cout << "D3DResource: " << d3dresource << endl;  
#endif

}

int sb_clGetContextWithDevice ( cl_platform_id ** platform, cl_device_id ** device, cl_context *context)
{
  cl_uint num_platforms;
  cl_uint plafrom_id = 0;
  cl_uint num_devices;
  cl_uint device_id = 0;
  cl_int context_error_ret;

  /*
   * Get platform
   */
  if (clGetPlatformIDs (1, NULL, &num_platforms) < 0 ) {
    cerr << "Could not get number of platforms." << endl;
    return 0;
  }
  
  if ( num_platforms < 1 ) {
    cerr << "Did not find possible platforms" << endl;
    return 0;
  } 
    

  *platform = new cl_platform_id;
  if (*platform == NULL ) {
    cerr << "Error allocating memory for platform." << endl;
    return 0;
  }
  
  if ( clGetPlatformIDs (1, *platform, NULL) < 0 ) {
    cerr << "Error allocating platform." << endl;
    return 0;
  }

  /*
   * Get Device
   */
  if ( clGetDeviceIDs ( **platform, CL_DEVICE_TYPE_ALL, 0, NULL, &num_devices ) < 0 ) {
    cerr << "Failed to query for platform devices." << endl;
    return 0;
  }

  *device = new cl_device_id;

  if (num_devices < 1) {
    cerr << "Did not find any devices to use." << endl;
    return 0;
  }
    
  if ( clGetDeviceIDs (**platform, CL_DEVICE_TYPE_ALL,
		       1, *device, NULL) < 0) {
    cerr << "Failed to get requested device." << endl;
    return 0;
  }

  /*
   * Get context
   */
  *context = clCreateContext ( NULL,
			      1, *device,
			      NULL, NULL, &context_error_ret );
  
  if ( ! (*context != 0 && context_error_ret == CL_SUCCESS)) {
    cerr << "Error creating a context." << endl;
    return 0;
  }
  
  return 1;
}
