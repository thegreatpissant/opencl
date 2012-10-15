ch2: chpter2.cpp
	g++ $(pkg-config --cflags --libs opencl-utils) -o ch2 chpter2.cpp

all: ch2
ch2a: chpter2.cpp
	g++ -o chpter2 chpter2.cpp -I/usr/include/opencl-utils  -lclrun  
