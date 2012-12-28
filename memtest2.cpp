/*
 * James A. Feister openjaf@gmail.com
 * A c++ implementation of the opencl version
 */

#include <iostream>

using namespace std;

int main () 
{

#define NUM_ITEMS 1000000
  
  float a[NUM_ITEMS], b[NUM_ITEMS];

  for (int i = 0; i < NUM_ITEMS; ++i) {
    a[i] = 2;
  }

  for (int i = 0; i < NUM_ITEMS; ++i)
    b[i] = a[i] * 2;

}
