all: chp2_platforms


sb_opencl.o: sb_opencl.h sb_opencl.cpp
	g++ -c sb_opencl.cpp -lOpenCL

chp2_platforms: chp2_platforms.cpp sb_opencl.o
	g++ -o chp2_platforms chp2_platforms.cpp -lOpenCL ./sb_opencl.o

ch2: chpter2.cpp
	g++ -o chpter2 chpter2.cpp -I/usr/include/opencl-utils  -lOpenCL -g

clean: 
	rm chpter2 chp2_platforms *.o
