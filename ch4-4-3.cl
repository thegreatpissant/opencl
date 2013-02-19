__kernel void memstuff( __global char16 *msg) 
{  

  *msg = (char16) ('h', 'e', 'l', 'l', 'o', 'P', 'r', 'o', 'g', 'r', 'a', 'm', 'm', 'e', 'r', '\\0'); 

}
