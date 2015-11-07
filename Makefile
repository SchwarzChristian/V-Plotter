CC_C    = gcc
CC_CPP  = g++
CFLAGS  = -Wall -g -fPIC -std=c++0x
CLIBS   = 
AR	= ar

controller: bin/controller Makefile
simulator: bin/simulator Makefile


# binaries

bin/simulator: obj/main.o obj/control_sim.o obj/coord.o obj/point.o
	$(CC_CPP) -o $@ $(CLIBS) -lGL -lGLU -lglut $^

bin/controller: obj/main.o bin/libvplotter.so
	$(CC_CPP) -o $@ $(CLIBS) -Lbin -lvplotter $<


# libraries

bin/libvplotter.so: obj/libvplotter.o obj/control_wp.o obj/coord.o obj/point.o
	$(CC_CPP) -shared -o $@ $^

# objects

obj/libvplotter.o: src/libvplotter.cpp src/libvplotter.hpp
	$(CC_CPP) -c -o $@ $(CFLAGS) $<

obj/control_wp.o: src/control_wp.cpp src/control.hpp Makefile
	$(CC_CPP) -c -o $@ $(CFLAGS) -lwiringPi $<

obj/control_sim.o: src/control_sim.cpp src/control.hpp Makefile
	$(CC_CPP) -c -o $@ $(CFLAGS)  $<

obj/coord.o: src/coord.cpp src/coord.hpp src/point.hpp src/control.hpp Makefile
	$(CC_CPP) -c -o $@ $(CFLAGS)  $<

obj/point.o: src/point.cpp src/point.hpp Makefile
	$(CC_CPP) -c -o $@ $(CFLAGS)  $<

obj/main.o: src/main.cpp src/control.hpp src/coord.hpp src/point.hpp Makefile
	$(CC_CPP) -c -o $@ $(CFLAGS)  $<

