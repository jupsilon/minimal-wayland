
.PHONY: clean

CXX=clang++
CXXFLAG=-std=c++1z

all: minimal-compositor minimal-shell

.cc.o: Makefile *.cc
	$(CXX) $(CXXFLAG) -c $< -o $@

minimal-compositor: Makefile minimal-compositor.o
	$(CXX) $(CXXFLAG) -o minimal-compositor minimal-compositor.o

minimal-shell: Makefile minimal-shell.o
	$(CXX) $(CXXFLAG) -o minimal-shell minimal-shell.o

clean:
	-rm -f *.o
	-rm -f minimal-compositor
	-rm -f minimal-shell
