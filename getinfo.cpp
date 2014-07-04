#include <iostream>
#include <CL/cl.hpp>
#include <vector>

using namespace std;
void sb_print_cl_image_format (cl_image_format *image_format) ;
//  Should have did this earlier....
#define ERROR_CASE(SWITCH_CASE, ERROR_MESSAGE) \
  case SWITCH_CASE:			       \
  cerr << ERROR_MESSAGE << endl;	       \
  break					       \

#define INFO_CASE(SWITCH_CASE, INFO_MESSAGE) \
  case SWITCH_CASE:			       \
  cout << INFO_MESSAGE << endl;		       \
  break					       \


int main () {

  vector <cl::Platform> platforms;
  vector <cl::Device>  platformDevices;

  cl::Platform::get (&platforms);
  platforms[0].getDevices (CL_DEVICE_TYPE_ALL, &platformDevices);
  cl::Context Context (platformDevices);

  cout << "Number of platforms: " << platforms.size() << endl;

  for (auto p: platforms) {
    cout << "Platform information" << endl;
    cout << "\tProfile: " << p.getInfo <CL_PLATFORM_PROFILE>() << endl
         << "\tVersion: " << p.getInfo <CL_PLATFORM_VERSION>() << endl
         << "\tName: "    << p.getInfo <CL_PLATFORM_NAME>() << endl
         << "\tVendor: "  << p.getInfo <CL_PLATFORM_VENDOR>() << endl
         << "\tExtensions: " << p.getInfo <CL_PLATFORM_EXTENSIONS>() << endl;
    
  }

  vector <cl::ImageFormat> imageFormats;
  cl_int ret = Context.getSupportedImageFormats( CL_MEM_READ_WRITE, CL_MEM_OBJECT_IMAGE2D, &imageFormats);
  if (ret == CL_INVALID_VALUE || ret == CL_OUT_OF_RESOURCES || ret == CL_OUT_OF_HOST_MEMORY) {
    cerr << "Failed to get image formats for devices." << endl;
    exit (EXIT_FAILURE);
  }
  cout << "Got back " << imageFormats.size() << " formats." << endl;
  for ( auto f : imageFormats ) {
    sb_print_cl_image_format (&f);
  }
}

 void sb_print_cl_image_format (cl_image_format *image_format) 
{
  cout << "\nImage format" << endl;
  cout << "\tchannel_order ";
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
    cout << "Unknown Image Format" << endl;
    break;
  }

  //  cout << "size of channel data type" << endl;
  cout << "\tData type:";
  switch (image_format->image_channel_data_type) {
    INFO_CASE (CL_SNORM_INT8,
	       "CL_SNORM_INT8, Each channel component is a normalized signed 8-bit integer value.");
    INFO_CASE (CL_SNORM_INT16,
	       "CL_SNORM_INT16, Each channel component is a normalized signed 16-bit integer value.");
    INFO_CASE (CL_UNORM_INT8,
	       "CL_UNORM_INT8, Each channel component is a normalized unsigned 8-bit integer value.");
    INFO_CASE (CL_UNORM_INT16,
	       "CL_UNORM_INT16, Each channel component is a normalized unsigned 16-bit integer value.");
    INFO_CASE (CL_UNORM_SHORT_565,
	       "CL_UNORM_SHORT_565, Represents a normalized 5-6-5 3-channel RGB image. The channel order must be CL_RGB.");
    INFO_CASE (CL_UNORM_SHORT_555,
	       "CL_UNORM_SHORT_555, Represents a normalized x-5-5-5 4-channel xRGB image. The channel order must be CL_RGB.");
    INFO_CASE (CL_UNORM_INT_101010,
	       "CL_UNORM_INT_101010, Represents a normalized x-10-10-10 4-channel xRGB image. The channel order must be CL_RGB.");
    INFO_CASE (CL_SIGNED_INT8,
	       "CL_SIGNED_INT8, Each channel component is an unnormalized signed 8-bit integer value.");
    INFO_CASE (CL_SIGNED_INT16,
	       "CL_SIGNED_INT16, Each channel component is an unnormalized signed 16-bit integer value.");
    INFO_CASE (CL_SIGNED_INT32,
	       "CL_SIGNED_INT32, Each channel component is an unnormalized signed 32-bit integer value.");
    INFO_CASE (CL_UNSIGNED_INT8,
	       "CL_UNSIGNED_INT8, Each channel component is an unnormalized unsigned 8-bit integer value.");
    INFO_CASE (CL_UNSIGNED_INT16,
	       "CL_UNSIGNED_INT16, Each channel component is an unnormalized unsigned 16-bit integer value.");
    INFO_CASE (CL_UNSIGNED_INT32,
	       "CL_UNSIGNED_INT32, Each channel component is an unnormalized unsigned 32-bit integer value.");
    INFO_CASE (CL_HALF_FLOAT,
	       "CL_HALF_FLOAT,Each channel component is a 16-bit half-float value.");
    INFO_CASE (CL_FLOAT,
	       "CL_FLOAT, Each channel component is a single precision floating-point value.");
  default:
    break;
  }

}
