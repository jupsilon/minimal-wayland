
.PHONY: clean

CXX=clang++
CXXFLAGS=-Wall -std=c++1z
CC=clang
CFLAGS=-Wall
GEN=wayland-scanner

all: minimal-compositor minimal-shell

run: all
	./minimal-compositor minimal-shell

gen: Makefile minimal-wayland.xml
	wayland-scanner code          < minimal-wayland.xml > minimal-wayland-protocol.c
	wayland-scanner server-header < minimal-wayland.xml > minimal-wayland-server-protocol.h
	wayland-scanner client-header < minimal-wayland.xml > minimal-wayland-client-protocol.h

.c.o: gen *.c *.h
	$(CC) $(CFLAGS) -c $< -o $@

.cc.o: *.cc *.hpp *.c
	$(CXX) $(CXXFLAGS) -c $< -o $@

minimal-compositor: Makefile minimal-compositor.o minimal-wayland-protocol.o
	$(CXX) $(CXXFLAGS) -o minimal-compositor \
	minimal-compositor.o \
	minimal-wayland-protocol.o \
	-lwayland-server

minimal-shell: Makefile minimal-shell.o minimal-wayland-protocol.o
	$(CXX) $(CXXFLAGS) -o minimal-shell \
	minimal-shell.o \
	minimal-wayland-protocol.o \
	-lwayland-client

clean:
	-rm -f *.o
	-rm minimal-wayland-protocol.c
	-rm minimal-wayland-server-protocol.h
	-rm minimal-wayland-client-protocol.h
	-rm -f minimal-compositor
	-rm -f minimal-shell
