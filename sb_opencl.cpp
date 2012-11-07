#include <CL/cl.h>
#include <iostream>
#include <cstdlib>
#include <string>
#include <stdio.h>

using std::cout;
using std::cerr;
using std::endl;
using std::string;

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
  /*
    CL_DEVICE_VENDOR             | char[]
    CL_DEVICE_NAME               | char[]
    CL_DEVICE_EXTENSIONS         | char[]
    CL_DEVICE_GLOBAL_MEM_SIZE    | cl_ulong
    CL_DEVICE_ADDRESS_BITS       | cl_uint
    CL_DEVICE_AVAILABLE          | cl_bool
    CL_DEVICE_COMPILER_AVAILABLE | cl_bool
  */

  size_t   qstring_len = 2048;
  cl_ulong qmemory_size = 0;
  cl_uint  qaddress_space = 0;
  cl_bool  qdevice_available = false;
  cl_bool  qdevice_compiler  = false;
  cl_char * qstring = (cl_char *) malloc (qstring_len+1);

  qstring[qstring_len] = '\0';

  clGetDeviceInfo ( *device, CL_DEVICE_VENDOR, qstring_len, qstring, NULL);
  cout << "Vendor Name: " << qstring << endl;

  clGetDeviceInfo ( *device, CL_DEVICE_NAME, qstring_len, qstring, NULL);
  cout << "Vendor Device Name: " <<  qstring << endl;

  clGetDeviceInfo ( *device, CL_DEVICE_EXTENSIONS, qstring_len, qstring, NULL);
  cout << "Device Extensions: " << qstring << endl;

  clGetDeviceInfo ( *device, CL_DEVICE_GLOBAL_MEM_SIZE, sizeof (cl_ulong), &qmemory_size, NULL);
  cout << "Device Address Space Size: " << qmemory_size << endl;

  clGetDeviceInfo ( *device, CL_DEVICE_ADDRESS_BITS, sizeof (cl_uint), &qaddress_space, NULL);
  cout << "Device Address Bits: " << qaddress_space << endl;

  clGetDeviceInfo ( *device, CL_DEVICE_AVAILABLE, sizeof (cl_bool), &qdevice_available, NULL);
  cout << "Device Available: " << (qdevice_available ? "TRUE":"FALSE" ) << endl;

  clGetDeviceInfo ( *device, CL_DEVICE_COMPILER_AVAILABLE, sizeof (cl_bool), &qdevice_compiler, NULL);
  cout << "Device Compiler: " << (qdevice_compiler ? "TRUE":"FALSE") << endl;

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
  cout << "Program: " << *program_buffer << endl;
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
    case CL_INVALID_CONTEXT :
      cerr << "Source Program: Invalid Context\n";
      break;
    case CL_INVALID_VALUE :
      cerr << "Source Program: Invalid Value\n";
      break;
    case CL_OUT_OF_HOST_MEMORY:
      cerr << "Source Program: Out of host memory\n";
      break;
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

  program_ret = clBuildProgram (*program, 1, devices, options, (void(*)(const char*, const void*, size_t, void*))callback, user_data);
  
  switch (program_ret) {
  case CL_SUCCESS:
    cout << "Program built without errors." << endl;
    break;
  case CL_INVALID_PROGRAM:
    cerr << "Error: Invalid Program Object." << endl;
    break;
  case CL_INVALID_VALUE:
    cerr << "Error: Invalid Value for devices." << endl;
    break;
  case CL_INVALID_DEVICE:
    cerr << "Error: Invalid device list." << endl;
    break;
  case CL_INVALID_BINARY:
    cerr << "Error: Ivalid Binary for devices." << endl;
    break;
  case CL_INVALID_BUILD_OPTIONS:
    cerr << "Error: Invalid build options." << endl;
    break;
    //  case CL_INVALID_OPERATION:
    //    cerr << "Error: Program build did not complete." << endl;
    //    break;
  case CL_COMPILER_NOT_AVAILABLE:
    cerr << "Error: Source compilation not available for device." << endl;
    break;
  case CL_BUILD_PROGRAM_FAILURE:
    cerr << "Error: Failure to build program." << endl;
    break;
  case CL_INVALID_OPERATION:
    cerr << "Error: Kernel objects already attached to program." << endl;
    break;
  case CL_OUT_OF_RESOURCES:
    cerr << "Error: Failure to allocate required resources." << endl;
    break;
  case CL_OUT_OF_HOST_MEMORY:
    cerr << "Error: Failure to allocate memory resources." << endl;
    break;
default:
    cerr << "Error: Unknown error building Program." << endl;
    break;
  }
  if (program_ret != CL_SUCCESS) {
    size_t build_param_size;
    char * build_param_value;

    clGetProgramBuildInfo (*program, devices[0],
			   CL_PROGRAM_BUILD_LOG,
			   1, NULL,
			   &build_param_size);
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

cl_kernel sb_clCreateKernel (cl_program program, char * function_name)
{
  cl_kernel kernel;
  cl_int kern_error;

  kernel = clCreateKernel (program, function_name, &kern_error);
  if (kernel == NULL) {
    switch(kern_error) {
    case CL_INVALID_PROGRAM: 
      cerr << "Program is not a valid program object." << endl;
      break;
    case CL_INVALID_PROGRAM_EXECUTABLE: 
      cerr << "There is no successfully built executable for program." << endl;
      break;
    case CL_INVALID_KERNEL_NAME: 
      cerr << "kernel_name \"" << function_name << "\" is not found in program." << endl;
      break;
    case CL_INVALID_KERNEL_DEFINITION:
      cerr << "The function definition for __kernel function given by kernel_name such" << endl
	   << "as the number of arguments, the argument types are not the same for all" << endl
	   << "devices for which the program executable has been built." << endl;
      break;
    case CL_INVALID_VALUE: 
      cerr << "kernel_name is NULL." << endl;
      break;
    case CL_OUT_OF_RESOURCES: 
      cerr << "Failure to allocate resources required by the OpenCL implementation on the device." << endl;
      break;
    case CL_OUT_OF_HOST_MEMORY:
      cerr << "Failure to allocate resources required by the OpenCL implementation on the host." << endl;
      break;
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
    case CL_INVALID_CONTEXT: 
      cerr << "Context is not a valid context." << endl;
      break;
    case CL_INVALID_DEVICE: 
      cerr << "Device is not a valid device or is not associated with context." << endl;
      break;
    case CL_INVALID_VALUE: 
      cerr << "Values specified in properties are not valid." << endl;
      break;
    case CL_INVALID_QUEUE_PROPERTIES:
      cerr << "Values specified in properties are valid but are not supported by the device." << endl;
      break;
    case CL_OUT_OF_RESOURCES:
      cerr << "Failure to allocate resources required by the OpenCL implementation on the device." << endl;
      break;
    case CL_OUT_OF_HOST_MEMORY: 
      cerr << "Failure to allocate resources required by the OpenCL implementation on the host." << endl;
      break;
    default:
      cerr << "Unknown error." << endl;;
      break;
    }
  }
  return command_queue;
}
