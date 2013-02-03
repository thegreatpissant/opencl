CXX=clang++
CC=clang
LIBS=-lOpenCL
STD=-std=c++11
OPT=-O3
# DEBUG=-g

EXECS=chp2_platforms memtest varinfo memtest2 hello_kernel ch5_vector_test chpt8_oo

all: sb_opencl.o chp2_platforms memtest memtest2 varinfo hello_kernel ch5_vector_test chpt8_oo

sb_opencl.o: sb_opencl.h sb_opencl.cpp
	$(CXX) -c sb_opencl.cpp $(STD) $(OPT) $(DEBUG)

chp2_platforms: chp2_platforms.cpp sb_opencl.o
	$(CXX) -o chp2_platforms chp2_platforms.cpp $(LIBS) $(STD) $(OPT) $(DEBUG) ./sb_opencl.o 

memtest: memtest.cpp sb_opencl.o
	$(CXX) -o memtest memtest.cpp -I/usr/include/opencl-utils $(LIBS) $(STD) $(OPT) $(DEBUG) ./sb_opencl.o 

memtest2: memtest2.cpp
	$(CXX) -o memtest2 memtest2.cpp $(LIBS) $(STD) $(OPT) $(DEBUG)

varinfo: varinfo.cpp sb_opencl.o
	$(CXX) -o varinfo varinfo.cpp -I/usr/include/opencl-utils  $(LIBS) $(STD) $(OPT) $(DEBUG) ./sb_opencl.o

hello_kernel: hello_kernel.cpp sb_opencl.o
	$(CXX) -o hello_kernel hello_kernel.cpp $(LIBS) $(STD) $(OPT) $(DEBUG)  ./sb_opencl.o 

ch5_vector_test: ch5_vector_test.cpp sb_opencl.o
	$(CXX) -o ch5_vector_test ch5_vector_test.cpp $(LIBS) $(STD) $(OPT) $(DEBUG) ./sb_opencl.o 

chpt8_oo: chpt8_oo.cpp 
	$(CXX) -std=c++11 -o chpt8_oo chpt8_oo.cpp $(LIBS) $(STD) $(OPT) $(DEBUG)

clean: 
	rm $(EXECS) *.o
