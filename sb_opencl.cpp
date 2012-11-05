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
  program_buffer->copy(source, program_buffer->length());
  program = clCreateProgramWithSource (context, 1, (const char **) &source, NULL, &error_code);
  if ( program == NULL ) {
    switch (error_code) {
    case CL_INVALID_CONTEXT :
      perror ("Source Program: Invalid Context\n");
      break;
    case CL_INVALID_VALUE :
      perror ("Source Program: Invalid Value\n");
      break;
    case CL_OUT_OF_HOST_MEMORY:
      perror ("Source Program: Out of host memory\n");
      break;
    default:
      perror ("Source Program: Unknown error\n");
      break;
    }
    return NULL;
  }

  return program;
} 
