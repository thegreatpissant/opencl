#include <stdlib.h>
#include <stdio.h>

#include <opencl-utils/include/CL/cl.h>
#include <opencl-utils/include/clrun.h>

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
  }

  free (platforms);
  exit (0);
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
