# QUICKSTART:
# * `make static` (default): build a statically-linked library in bin/
# * `make shared`: build a dynamically-linked library in bin/i
# * `make all`: builds the static and static-example targets
# * `make debug`: same as `make all` but with debugging flags set
# * `make example`, `make static-example`: build all example programs and link them 
# 	statically.
# * `make dynamic-example`: build all example programs and link them dynamically.
# 	Be sure to run `make dynamic` first (for the same reason as above).

# Project root (Fix for Windows compilation)
PROJ_ROOT = $(dir $(abspath $(lastword $(MAKEFILE_LIST))))

# Where to find the sources
LIB_SRCS = $(wildcard *.cpp) $(wildcard tools/*.cpp)
BUILTIN_COMMANDS = $(wildcard cmd/*.cpp)
EXAMPLE_SRCS = $(wildcard examples/*.cpp)
EXAMPLE_FILES = $(notdir $(EXAMPLE_SRCS))

# Output directories
BIN_ROOT = bin
BIN_OBJ = $(BIN_ROOT)/obj
BIN_LIB = $(BIN_ROOT)
BIN_EXAMPLE = $(BIN_ROOT)/example

# Output objects
LIB_OBJS = $(addprefix $(BIN_OBJ)/, $(LIB_SRCS:%.cpp=%.o))
BUILTIN_OBJS = $(addprefix $(BIN_OBJ)/, $(BUILTIN_COMMANDS:%.cpp=%.o))
STATIC_LIB = $(BIN_LIB)/libiash.a
DYNAMIC_LIB = $(BIN_LIB)/libiash.so
EXAMPLES = $(addprefix $(BIN_EXAMPLE)/, $(EXAMPLE_FILES:%.cpp=%))
EXAMPLE_PREREQ = 

# Executables and flags
CXX = g++
CXXFLAGS = -Wall -Werror -pedantic --std=c++11
OPTIMIZATION = -O2
LIBFLAGS = 
EXAMPLE_LINKFLAGS = -L$(abspath $(BIN_LIB)) -liash
AR = ar
MKDIR = mkdir -p

.PHONY: clean all static dynamic example static-example dynamic-example

ifeq ($(OS),Windows_NT)
    MKDIR = md
endif

static: $(STATIC_LIB)

dynamic: LIBFLAGS += -fpic
dynamic: $(DYNAMIC_LIB)

debug: OPTIMIZATION = -g
debug: all

all: static static-example

$(STATIC_LIB): $(LIB_OBJS) $(BUILTIN_OBJS)
	$(AR) rcs "$@" $(LIB_OBJS) $(BUILTIN_OBJS)

$(DYNAMIC_LIB): $(LIB_OBJS) $(BUILTIN_OBJS)
	$(CXX) $(LIB_OBJS) $(BUILTIN_OBJS) -shared -o "$@" 

$(BIN_OBJ)/%.o: %.cpp 
	@$(MKDIR) $(dir $@)
	$(CXX) $(CXXFLAGS) $(LIBFLAGS) $(OPTIMIZATION) -c $< -o $@

example: static-example

static-example: $(addsuffix .static, $(EXAMPLES))

dynamic-example: $(addsuffix .dynamic, $(EXAMPLES))
	
$(BIN_EXAMPLE)/%.static: examples/%.cpp static | $(BIN_EXAMPLE) 
	$(CXX) $(CXXFLAGS) $(OPTIMIZATION) -I $(PROJ_ROOT) $< $(STATIC_LIB) -o $@
	
$(BIN_EXAMPLE)/%.dynamic: examples/%.cpp dynamic | $(BIN_EXAMPLE)
	$(CXX) $(CXXFLAGS) $(OPTIMIZATION) -I $(PROJ_ROOT) $< -o $@ $(EXAMPLE_LINKFLAGS)

$(BIN_EXAMPLE):
	@$(MKDIR) $@

clean:
	rm -vrf $(BIN_ROOT)
