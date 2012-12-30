EXECS=chp2_platforms memtest varinfo memtest2 hello_kernel ch5_vector_test

all: sb_opencl.o chp2_platforms memtest memtest2 varinfo hello_kernel ch5_vector_test

sb_opencl.o: sb_opencl.h sb_opencl.cpp
	$(CXX) -c sb_opencl.cpp -lOpenCL

chp2_platforms: chp2_platforms.cpp sb_opencl.o
	$(CXX) -o chp2_platforms chp2_platforms.cpp -lOpenCL ./sb_opencl.o

memtest: memtest.cpp sb_opencl.o
	$(CXX) -o memtest memtest.cpp -I/usr/include/opencl-utils  -lOpenCL -g ./sb_opencl.o

memtest2: memtest2.cpp
	$(CXX) -o memtest2 memtest2.cpp 

varinfo: varinfo.cpp
	$(CXX) -o varinfo varinfo.cpp -I/usr/include/opencl-utils  -lOpenCL -g

hello_kernel: hello_kernel.cpp sb_opencl.o
	$(CXX) -o hello_kernel hello_kernel.cpp -lOpenCL ./sb_opencl.o

ch5_vector_test: ch5_vector_test.cpp sb_opencl.o
	$(CXX) -o ch5_vector_test ch5_vector_test.cpp -lOpenCL ./sb_opencl.o -Wall

clean: 
	rm $(EXECS) *.o
