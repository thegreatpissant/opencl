#include <iostream>
#include <vector>
#include <CL/cl.hpp>

using namespace std;

int main () {

  vector <cl::Platform> Platforms;
  vector <cl::Device>   Devices;

  cl::Platform::get ( &Platforms );
  if (Platforms.size() <= 0) {
    cerr << "No platforms found" << endl;
    exit (EXIT_FAILURE);
  }
  cout << "Got " << Platforms.size() << " platforms.";
  cout << "Using first on list.";
  auto p = Platforms[0];
  cout << "Platform info: " << endl;
  cout << "\tProfile: " << p.getInfo <CL_PLATFORM_PROFILE>() << endl
       << "\tVersion: " << p.getInfo <CL_PLATFORM_VERSION>() << endl
       << "\tName: "    << p.getInfo <CL_PLATFORM_NAME>() << endl
       << "\tVendor: "  << p.getInfo <CL_PLATFORM_VENDOR>() << endl
       << "\tExtensions: " << p.getInfo <CL_PLATFORM_EXTENSIONS>() << endl;

  Platforms[0].getDevices (CL_DEVICE_TYPE_ALL, &Devices);
  if (Devices.size() <= 0) {
    cerr << "Platforms has no devices available." << endl;
    exit (EXIT_FAILURE);
  }
  cout << "Got " << Devices.size() << endl;
  cout << "Using first available" << endl;
  auto d = Devices[0];
  cout << "Device info: " << endl
       << "\tName: " << d.getInfo <CL_DEVICE_NAME>() << endl
       << "\tVendor: " << d.getInfo <CL_DEVICE_VENDOR>() << endl
       << "\tProfile: " << d.getInfo <CL_DEVICE_PROFILE>() << endl
       << "\tVersion: " << d.getInfo <CL_DEVICE_VERSION>() << endl
       << "\tDriver_Version: " << d.getInfo <CL_DRIVER_VERSION>() << endl
       << "\tExtensions: " << d.getInfo <CL_DEVICE_EXTENSIONS>() << endl
       << "\tCompute Units: " << d.getInfo <CL_DEVICE_MAX_COMPUTE_UNITS>() << endl
       << "\tItem Dimensions: " << d.getInfo <CL_DEVICE_MAX_WORK_ITEM_DIMENSIONS>() << endl
       << "\tWork Group size: " << d.getInfo <CL_DEVICE_MAX_WORK_GROUP_SIZE>() << endl
       << "\tWork Item size: " << d.getInfo <CL_DEVICE_MAX_WORK_GROUP_SIZE>() << endl;
}
