CXXFLAGS = -Wall -v -O2 -march=native
DEBUG = -Wall -g
MAKEFLAGS = -j2

# Let's use all .cc files as targets
objects = $(patsubst src/%.cc, src/%.o, $(wildcard src/*.cc))

all: ssRay
debug: CXXFLAGS = $(DEBUG)
debug: ssRay
ssRay: $(objects) 
	$(CXX) $(objects) -lrt -pthread -lX11 -o $@

# All objects depend on associated .h file, except main.o
$(filter-out main.o, $(objects)):  %.o: %.h

.PHONY : clean
clean :
	$(RM) src/*.o src/*.s
.PHONY : rebuild
rebuild: 
	$(RM) src/*.o && make

