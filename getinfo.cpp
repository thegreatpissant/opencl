#include <iostream>
#include <CL/cl.hpp>
#include <vector>

using namespace std;

int main () {

  vector <cl::Platform> platforms;

  cl::Platform::get (&platforms);

  cout << "Number of platforms: " << platforms.size() << endl;

  for (auto p: platforms) {
    cout << "Platform information" << endl;
    cout << "\tProfile: " << p.getInfo <CL_PLATFORM_PROFILE>() << endl
         << "\tVersion: " << p.getInfo <CL_PLATFORM_VERSION>() << endl
         << "\tName: "    << p.getInfo <CL_PLATFORM_NAME>() << endl
         << "\tVendor: "  << p.getInfo <CL_PLATFORM_VENDOR>() << endl
         << "\tExtensions: " << p.getInfo <CL_PLATFORM_EXTENSIONS>() << endl;
    
  }

}
