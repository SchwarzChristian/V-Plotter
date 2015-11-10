CC_C    = gcc
CC_CPP  = g++
CFLAGS  = -Wall -g -fPIC -std=c++0x
CLIBS   = 


controller: bin/controller
simulator: bin/libvsimulator.so bin/simulator


# binaries

bin/simulator: obj/main.o bin/libvsimulator.so
	$(CC_CPP) -o $@ $(CLIBS) -Lbin -lvsimulator $<

bin/controller: obj/main.o bin/libvplotter.so
	$(CC_CPP) -o $@ $(CLIBS) -Lbin -lvplotter $<


# libraries

bin/libvplotter.so: obj/libvplotter.o obj/control_wp.o obj/coord.o obj/point.o
	$(CC_CPP) -shared -o $@ -lwiringPi $^

bin/libvsimulator.so: obj/libvplotter.o obj/control_sim.o obj/coord.o obj/point.o
	$(CC_CPP) -shared -o $@ -lGL -lGLU -lglut $^

# objects

obj/libvplotter.o: src/libvplotter.cpp src/vplotter.h
	$(CC_CPP) -c -o $@ $(CFLAGS) $<

obj/control_wp.o: src/control_wp.cpp src/control.hpp Makefile
	$(CC_CPP) -c -o $@ $(CFLAGS) -lwiringPi $<

obj/control_sim.o: src/control_sim.cpp src/control.hpp Makefile
	$(CC_CPP) -c -o $@ $(CFLAGS)  $<

obj/coord.o: src/coord.cpp src/coord.hpp src/point.hpp src/control.hpp Makefile
	$(CC_CPP) -c -o $@ $(CFLAGS)  $<

obj/point.o: src/point.cpp src/point.hpp Makefile
	$(CC_CPP) -c -o $@ $(CFLAGS)  $<

obj/main.o: src/main.cpp src/vplotter.h
	$(CC_CPP) -c -o $@ $(CFLAGS)  $<


#misc

install: controller
	cp bin/libvplotter.so /usr/lib/
	cp bin/controller     /usr/bin/vplotter
	cp src/vplotter.h     /usr/include/

uninstall:
	rm -rf /usr/lib/libvplotter.so
	rm -rf /usr/bin/controller
	rm -rf /usr/include/vplotter.h

clean:
	rm -rf bin/*
	rm -rf obj/*
	rm -rf src/*~
