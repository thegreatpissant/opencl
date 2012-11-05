all: ch2
ch2: chpter2.cpp
	g++ -o chpter2 chpter2.cpp -I/usr/include/opencl-utils  -lOpenCL -g

clean: 
	rm chpter2
