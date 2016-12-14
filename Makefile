# Handwritten makefile for iash

# Where to find the sources
LIB_SRCS = $(wildcard src/*.cpp) $(wildcard src/tools/*.cpp)
BUILTIN_COMMANDS = $(wildcard src/cmd/*.cpp)
EXAMPLE_SRCS = $(wildcard examples/*.cpp)
EXAMPLE_FILES = $(notdir $(EXAMPLE_SRCS))

# Output directories
BIN_ROOT = bin
BIN_OBJ = $(BIN_ROOT)/obj
BIN_LIB = $(BIN_ROOT)/lib
BIN_EXAMPLE = $(BIN_ROOT)/example

# Output objects
LIB_OBJS = $(LIB_SRCS:src/%.cpp=$(BIN_OBJ)/%.o)
BUILTIN_OBJS = $(BUILTIN_COMMANDS:src/%.cpp=$(BIN_OBJ)/%.o)

# Executables
CXX = g++
CXXFLAGS = -Wall -Werror -pedantic --std=c++11
LIBFLAGS = -c -fpic
AR = ar

.PHONY: clean

all: iash example

iash: $(LIB_OBJS) $(BUILTIN_OBJS)
	mkdir -p $(BIN_LIB)
	$(AR) rcs "$(BIN_LIB)/lib$@.a" $(wildcard $(BIN_OBJ)/*.o)
	$(CXX) $(LIB_OBJS) $(BUILTIN_OBJS) -shared -o "$(BIN_LIB)/lib$@.so"

example: $(BIN_EXAMPLE)/$(EXAMPLE_FILES:.cpp=)

$(BIN_OBJ)/%.o: src/%.cpp
	mkdir -p $(dir $@)
	$(CXX) $(CXXFLAGS) $(LIBFLAGS) $< -o $@
	
$(BIN_EXAMPLE)/%: examples/%.cpp iash
	mkdir -p $(dir $@)
	$(CXX) $(CXXFLAGS) -L$(BIN_LIB) -liash $< -o $@
	
clean:
	rm -vrf $(BIN_ROOT)