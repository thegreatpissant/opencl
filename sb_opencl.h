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
#endif
