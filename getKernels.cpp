#define __CL_ENABLE_EXCEPTIONS
#include <iostream>
#include <fstream>
#include <iterator>
#include <CL/cl.hpp>
#include <vector>

using namespace std;


void CL_CALLBACK contexCallBack (const char * errInfo,
                                 const void *private_info_size,
                                 ::size_t cb,
                                 void *user_data ) {
  cerr << "Recieved an error from Context" << endl;
  cerr << "Error: " << errInfo << endl;
  exit (EXIT_FAILURE);
}

int main() {
  vector <cl::Platform> Platforms;
  vector <cl::Device> Devices;

  try {
    cl::Platform::get(&Platforms);
    Platforms[0].getDevices (CL_DEVICE_TYPE_ALL, &Devices);
    cl::Context Context (Devices);
    ifstream programFile ("getKernels.cl");
    string programString (istreambuf_iterator<char> (programFile),
                          (istreambuf_iterator<char>()));
    cout << "Program: \n" << programString << endl;
    cl::Program::Sources Source (1, make_pair (programString.c_str(), programString.length () +1));
    cl::Program Program (Context, Source);
    vector<cl::Kernel> AllKernels;
    Program.build (Devices);
    Program.createKernels (&AllKernels);
    for (auto k: AllKernels) {
      cout << "Kernel Name: " << k.getInfo<CL_KERNEL_FUNCTION_NAME>() << endl;
    }
  }
  catch (cl::Error e) {
    cerr << "Error: " << e.what() << " : Error Code " << e.err() << endl;
  }

}
