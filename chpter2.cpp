#include <stdlib.h>
#include <stdio.h>

#include <CL/cl.h>
//#include <opencl-utils/include/clrun.h>
void sb_clPrintContextInfo ( cl_context * context );
void sb_clPrintPlatformDevices ( cl_platform_id * platform );
void sb_clPrintPlatformDeviceInfo ( cl_device_id * device );
void sb_clPrintPlatformInfo ( cl_platform_id * extension);
void sb_clPrintPlatformExtension ( cl_platform_id * platform, cl_int extension );
int  sb_clReadSourceProgramFromDisk ( char * file_name, char * program_buffer, int *program_size );
cl_program sb_clCreateProgramFromSource (char * program_buffer, int program_size, cl_context context );

int main (int argc, char ** argv)
{
  
  cl_int ret;
/*
  if (( ret = clrInit() ))
    perror ("ERROR: Initializing OpenCL\n");
  printf ("Success! OpenCL %s\n", clrHasOpenCL() ? "exists!" : "does not exist. ");
*/

  cl_platform_id *platforms;
  cl_uint num_entries = 1;
  cl_uint num_platforms;

  if ( clGetPlatformIDs (1, NULL, &num_platforms) < 0 ) {
    perror ("Error getting number of platforms\n");
    exit (EXIT_FAILURE);
  }
  
  num_entries = num_platforms;
  platforms = (cl_platform_id *) malloc (sizeof (cl_platform_id) * num_entries);
  
  if ( clGetPlatformIDs (num_entries, platforms, NULL) < 0 ) {
    perror ("Error getting platforms\n");
    exit (EXIT_FAILURE);
  }
  printf ("num_platforms: %d\n", num_platforms);
  
  cl_uint i = 0;
  for (; i < num_platforms; ++i) {
    printf ("Platform #%d ", i);
    sb_clPrintPlatformInfo (platforms+i);
    sb_clPrintPlatformDevices (platforms+i);
  }

  //  Query for a context and print its information
  cl_int context_error;
  cl_context qcontext;
  cl_device_id * devices;
  cl_uint num_devices = 0;
  clGetDeviceIDs (*platforms, CL_DEVICE_TYPE_GPU, 1, NULL, &num_devices);
  devices = (cl_device_id *) malloc (sizeof (cl_device_id) *num_devices);
  clGetDeviceIDs (*platforms, CL_DEVICE_TYPE_GPU, num_devices, devices, NULL);
  qcontext = clCreateContext (NULL, 1, devices, NULL, NULL, &context_error);
  if (context_error < 0) {
    perror ("Error getting context\n");
    exit (EXIT_FAILURE);
  }
  sb_clPrintContextInfo (&qcontext);
  clRetainContext (qcontext);
  sb_clPrintContextInfo (&qcontext);
  clReleaseContext (qcontext);
  sb_clPrintContextInfo (&qcontext);
  clReleaseContext (qcontext);

  //  Free the platforms and exit
  free (platforms);
  exit (0);
}
void sb_clPrintPlatformDevices ( cl_platform_id * platform )
{
  cl_uint num_devices = 0;
  cl_device_id * devices;

  // Get  CL_DEVICE_TYPE_DEFAULT for the platform
  clGetDeviceIDs (*platform, CL_DEVICE_TYPE_DEFAULT, 1, NULL, &num_devices);
  printf ("DEFAULT platform devices: %d\n", num_devices);

  // Get  CL_DEVICE_TYPE_CPU for the platform
  clGetDeviceIDs (*platform,  CL_DEVICE_TYPE_CPU, 1, NULL, &num_devices);
  printf ("CPU platform devices: %d\n", num_devices);

  // Get  CL_DEVICE_TYPE_GPU for the platform
  clGetDeviceIDs (*platform,  CL_DEVICE_TYPE_GPU, 1, NULL, &num_devices);
  printf ("GPU platform devices: %d\n", num_devices);

  // Get  CL_DEVICE_TYPE_ACCELERATOR for the platform
  clGetDeviceIDs (*platform,  CL_DEVICE_TYPE_ACCELERATOR, 1, NULL, &num_devices);
  printf ("ACCELERATOR platform devices: %d\n", num_devices);

  //  Get CL_DEVICE_TYPE_ALL for the platform
  clGetDeviceIDs (*platform, CL_DEVICE_TYPE_ALL, 1, NULL, &num_devices);
  printf ("ALL platform devices: %d\n", num_devices);
  if (num_devices > 0)
    {
      devices = (cl_device_id*)malloc (sizeof (cl_device_id) * num_devices);
      clGetDeviceIDs (*platform, CL_DEVICE_TYPE_ALL, num_devices, devices, NULL);
      uint i = 0;
      for (; i < num_devices; ++i) {
	printf ("Device #%d ", i);
	sb_clPrintPlatformDeviceInfo (devices+i);
      }
    }
}

void sb_clPrintContextInfo ( cl_context * context )
{
  //  cl_uint num_devices;
  cl_uint ref_count;
  //  cl_bool d3d_resource;
  cl_int cerror;

  /*  This was added in the 1.1 version
  cerror =  clGetContextInfo (*context, CL_CONTEXT_NUM_DEVICES, sizeof (cl_uint), &num_devices, NULL);
  if (cerror < 0) {
    perror ("Error getting context CL_CONTEXT_NUM_DEVICES info\n");
    exit (EXIT_FAILURE);
  }
  printf ("Context number of devices: %d\n", num_devices);

  */
  cerror = clGetContextInfo (*context, CL_CONTEXT_REFERENCE_COUNT, sizeof (cl_uint), &ref_count, NULL);
  if (cerror < 0) {
    perror ("Error getting context CL_CONTEXT_REFERENCE_COUNT info\n");
    exit (EXIT_FAILURE);
  }
  printf ("Context reference count: %d\n", ref_count);

  /*  This was added in the 1.1 version
  cerror = clGetContextInfo (*context, CL_CONTEXT_D3D10_PREFER_SHARED_RESOURCES_KHR, sizeof (cl_bool), &d3d_resource, NULL);
  if (cerror < 0) {
    perror ("Error getting context CL_CONTEXT_D3D10_PREFER_SHARED_RESOURCES_KHR info\n");
    exit (EXIT_FAILURE);
  }
  printf ("Context d3d exchange: %s\n", d3d_resource ? "true":"false");
  */

}

void sb_clPrintPlatformDeviceInfo ( cl_device_id * device )
{
  /*
    CL_DEVICE_NAME               | char[]
    CL_DEVICE_VENDOR             | char[]
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

  clGetDeviceInfo ( *device, CL_DEVICE_NAME, qstring_len, qstring, NULL);
  printf ("Vendor Device Name: %s\n", qstring);

  clGetDeviceInfo ( *device, CL_DEVICE_VENDOR, qstring_len, qstring, NULL);
  printf ("Vendor Name: %s\n", qstring);

  clGetDeviceInfo ( *device, CL_DEVICE_EXTENSIONS, qstring_len, qstring, NULL);
  printf ("Device Extensions: %s\n", qstring);

  clGetDeviceInfo ( *device, CL_DEVICE_GLOBAL_MEM_SIZE, sizeof (cl_ulong), &qmemory_size, NULL);
  printf ("Device Address Space Size: %ld\n", qmemory_size);

  clGetDeviceInfo ( *device, CL_DEVICE_ADDRESS_BITS, sizeof (cl_uint), &qaddress_space, NULL);
  printf ("Device Address Bits: %d\n", qaddress_space);

  clGetDeviceInfo ( *device, CL_DEVICE_AVAILABLE, sizeof (cl_bool), &qdevice_available, NULL);
  printf ("Device Available: %s\n", qdevice_available ? "true":"false");

  clGetDeviceInfo ( *device, CL_DEVICE_COMPILER_AVAILABLE, sizeof (cl_bool), &qdevice_compiler, NULL);
  printf ("Device Compiler: %s\n", qdevice_compiler ? "true":"false");

  //  clGetDeviceInfo ( *device, CL_DEVICE_NAME, name_length, name, NULL);
  //  printf ("Device name: %s\n", name);

  /* 
     cl_int clGetDeviceInfo (cl_device_id device, cl_device_info
     param_name, size_t param_value_size, void *param_value, size_t
     *param_value_size_ret);
  */

}
void sb_clPrintPlatformInfo ( cl_platform_id * platform ) 
{
  printf ("Platform Info\n");

  printf ("\tName: ");
  sb_clPrintPlatformExtension ( platform, CL_PLATFORM_NAME );
  printf ("\n");

  printf ("\tVendor: ");
  sb_clPrintPlatformExtension (  platform, CL_PLATFORM_VENDOR );
  printf ("\n");

  printf ("\tVersion: ");
  sb_clPrintPlatformExtension (  platform, CL_PLATFORM_VERSION );
  printf ("\n");

  printf ("\tProfile: ");
  sb_clPrintPlatformExtension (  platform, CL_PLATFORM_PROFILE );
  printf ("\n");

  printf ("\tExtensions: ");
  sb_clPrintPlatformExtension (  platform, CL_PLATFORM_EXTENSIONS );
  printf ("\n");
}

void sb_clPrintPlatformExtension ( cl_platform_id * platform, cl_int extension ) 
{
  cl_char * ext_data;
  size_t param_size;
  cl_int ret;

  ret = clGetPlatformInfo (*platform, extension, 0, NULL, &param_size);
  if ( ret < 0 ) {
    perror ("Couldn't read extension data.");
    exit (EXIT_FAILURE);
  }
  ext_data = (cl_char*)malloc(param_size+1);
  ext_data[param_size] = '\0';
  ret = clGetPlatformInfo (*platform, extension, param_size, ext_data, NULL);
  printf ("%s", ext_data);
  free (ext_data);
}

int sb_clReadSourceProgramFromDisk ( char * file_name, char * program_buffer, int *program_size )
{
  long file_size;
  FILE *file_handle;
  file_handle = fopen (file_name, "r");
  if ( NULL == file_handle ) {
    perror ("Error opening file\n"); 
    return -1;
  }

  fseek (file_handle, 0, SEEK_END);
  file_size = ftell (file_handle);
  rewind (file_handle);
  
  program_buffer = (char *) malloc (file_size + 1);
  program_buffer [file_size] = '\0';
  fread (program_buffer, sizeof (char), file_size, file_handle);
  fclose (file_handle);
  *program_size = file_size;
  return file_size;
}

cl_program sb_clCreateProgramFromSource (char * program_buffer, size_t *program_size, cl_context context )
{
  cl_program program;
  cl_int error_code;
  program = clCreateProgramWithSource (context, 1, (const char **) &program_buffer, program_size, &error_code);
  if (NULL == program) {
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

cl_int sb_clBuildProgram (cl_program * program, 
			  cl_uint num_devices, const cl_device_id *device_list, 
			  const char *options, 
			  void *pfn_notify, 
			  void *user_data)
{
  clBuildProgram ( *program, num_devices, device_list, options, NULL, user_data);
  return 0;
}
