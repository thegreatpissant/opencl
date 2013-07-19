CXX=clang++
CC=clang
LIBS=-lOpenCL
STD=-std=c++11
OPT=-O3
DEBUG=-g

EXECS=chp2_platforms memtest varinfo memtest2 hello_kernel ch5_vector_test chpt8_oo memtest3 getKernels getContext getDeviceInfo ch4-4-3 ch5-1

all: sb_opencl.o chp2_platforms memtest memtest2 varinfo hello_kernel ch5_vector_test chpt8_oo memtest3 getKernels getContext getDeviceInfo ch4-4-3 ch5-1

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
	$(CXX) -o chpt8_oo chpt8_oo.cpp $(LIBS) $(STD) $(OPT) $(DEBUG)

memtest3: memtest3.cpp 
	$(CXX) -o memtest3 memtest3.cpp $(LIBS) $(STD) $(OPT) $(DEBUG)

getInfoCpp: getinfo.cpp sb_opencl.o
	$(CXX) -o getInfoCpp getinfo.cpp $(LIBS) $(STD) $(OPT) $(DEBUG) ./sb_opencl.o

getDeviceInfo: getDeviceInfo.cpp 
	$(CXX) -o getDeviceInfo getDeviceInfo.cpp $(LIBS) $(STD) $(OPT) $(DEBUG)

getContext: getContext.cpp 
	$(CXX) -o getContext getContext.cpp $(LIBS) $(STD) $(OPT) $(DEBUG)

getKernels: getKernels.cpp 
	$(CXX) -o getKernels getKernels.cpp $(LIBS) $(STD) $(OPT) $(DEBUG)

ch4-4-3: ch4-4-3.cpp 
	$(CXX) -o ch4-4-3 ch4-4-3.cpp $(LIBS) $(STD) $(OPT) $(DEBUG)

ch5-1: ch5-1.cpp 
	$(CXX) -o ch5-1 ch5-1.cpp $(LIBS) $(STD) $(OPT) $(DEBUG)

clean: 
	rm $(EXECS) *.o
