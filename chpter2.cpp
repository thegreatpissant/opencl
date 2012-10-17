#include <stdlib.h>
#include <stdio.h>

#include <opencl-utils/include/CL/cl.h>
#include <opencl-utils/include/clrun.h>
void sb_clPrintPlatformDevices ( cl_platform_id * platform );
void sb_clPrintPlatformDeviceInfo ( cl_device_id * device );
void sb_clPrintPlatformInfo ( cl_platform_id * extension);
void sb_clPrintPlatformExtension ( cl_platform_id * platform, cl_int extension );

int main (int argc, char ** argv)
{
  
  cl_int ret;

  if (( ret = clrInit() ))
    printf ("ERROR: %d\n", ret);
  printf ("Success! OpenCL %s\n", clrHasOpenCL() ? "exists!" : "does not exist. ");


  cl_platform_id *platforms;
  cl_uint num_entries = 1;
  cl_uint num_platforms;

  if ( clGetPlatformIDs (1, NULL, &num_platforms) < 0 ) {
    printf ("Error getting number of platforms\n");
    exit (EXIT_FAILURE);
  }
  
  num_entries = num_platforms;
  platforms = (cl_platform_id *) malloc (sizeof (cl_platform_id) * num_entries);
  
  if ( clGetPlatformIDs (num_entries, platforms, NULL) < 0 ) {
    printf ("Error getting platforms\n");
    exit (EXIT_FAILURE);
  }
  printf ("num_platforms: %d\n", num_platforms);
  
  cl_uint i = 0;
  for (; i < num_platforms; ++i) {
    printf ("Platform #%d ", i);
    sb_clPrintPlatformInfo (platforms+i);
    sb_clPrintPlatformDevices (platforms+i);
  }

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
      int i = 0;
      for (; i < num_devices; ++i) {
	printf ("Device #%d ", i);
	sb_clPrintPlatformDeviceInfo (devices+i);
      }
    }
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
  printf ("Device Address Space Size: %d\n", qmemory_size);

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
