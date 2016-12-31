CXXFLAGS = -Wall -v -O2 -march=native
DEBUG = -Wall -g
MAKEFLAGS = -j2

# Let's use all .cc files as targets
objects = $(patsubst %.cc, %.o, $(wildcard *.cc))

all: ssRay
debug: CXXFLAGS = $(DEBUG)
debug: ssRay
ssRay: $(objects) 
	$(CXX) $(objects) -lrt -pthread -lX11 -o $@

# All objects depend on associated .h file, except main.o
$(filter-out main.o, $(objects)):  %.o: %.h

.PHONY : clean
clean :
	$(RM) *.o *.s
.PHONY : rebuild
rebuild: 
	$(RM) *.o && make

