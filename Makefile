CC_C    = gcc
CC_CPP  = g++
CFLAGS  = -Wall -g -std=c++0x
CLIBS   = -Llib

all: bin/main Makefile

bin/main: obj/main.o obj/control.o obj/coord.o obj/point.o
	$(CC_CPP) -o $@ $(CLIBS)  $^

obj/control.o: src/control.cpp src/control.hpp Makefile
	$(CC_CPP) -c -o $@ $(CFLAGS)  $<

obj/coord.o: src/coord.cpp src/coord.hpp src/point.hpp src/control.hpp Makefile
	$(CC_CPP) -c -o $@ $(CFLAGS)  $<

obj/point.o: src/point.cpp src/point.hpp Makefile
	$(CC_CPP) -c -o $@ $(CFLAGS)  $<

obj/main.o: src/main.cpp src/control.hpp src/coord.hpp src/point.hpp Makefile
	$(CC_CPP) -c -o $@ $(CFLAGS)  $<

