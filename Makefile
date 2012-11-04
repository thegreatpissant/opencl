all: ch2
ch2: chpter2.cpp
	g++ -o chpter2 chpter2.cpp -I/usr/include/opencl-utils  -lOpenCL  

clean: 
	rm chpter2
