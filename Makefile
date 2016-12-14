# Handwritten makefile for iash

# Where to find the sources
LIB_SRCS = $(wildcard src/*.cpp) $(wildcard src/tools/*.cpp)
BUILTIN_COMMANDS = $(wildcard src/cmd/*.cpp)
EXAMPLE_SRCS = $(wildcard example/*.cpp)

# Output directories
BIN_ROOT = bin/
BIN_LIB = $(BIN_ROOT)/lib
BIN_EXAMPLE = $(BIN_ROOT)/example

CXX = g++
CXXFLAGS = -Wall -Werror -pedantic --std=c++11
LIBFLAGS = -c -fpic

.PHONY: clean

all: iash example

iash: $(LIB_SRCS) $(BUILTIN_COMMANDS)
	$(CXX) $(CXXFLAGS) $(LIBFLAGS) $^ -o "lib$@.o"
	$(CXX) -shared -o "lib$@.so" "lib$@.o"

examples: $(BIN_EXAMPLE)/$(notdir EXAMPLE_SRCS:.cpp=)
	
$(BIN_EXAMPLE)/%: $(EXAMPLE_SRCS)/%.cpp iash
	$(CXX) $(CXXFLAGS) -L$(BIN_LIB) -liash $< -o $@
	
clean:
	rm -vrf $(BIN_ROOT)