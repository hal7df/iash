# Handwritten makefile for iash

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

# Executables and flags
CXX = g++
CXXFLAGS = -Wall -Werror -pedantic --std=c++11
OPTIMIZATION = -O2
LIBFLAGS = 
EXEC_LINKFLAGS = 
EXEC_LINKLIBS = 
AR = ar
MKDIR = mkdir -p

.PHONY: clean all static dynamic example static-example dynamic-example

all: static

debug: OPTIMIZATION = -g
debug: static static-example

static: LINKFLAGS += $(STATIC_LIB)
static: $(STATIC_LIB)

dynamic: LIBFLAGS += -fpic
dynamic: LINKFLAGS += -L$(abspath $(BIN_LIB)) -liash
dynamic: $(DYNAMIC_LIB)

$(STATIC_LIB): $(LIB_OBJS) $(BUILTIN_OBJS)
	$(MKDIR) $(BIN_LIB)
	$(AR) rcs "$@" $(LIB_OBJS) $(BUILTIN_OBJS)

$(DYNAMIC_LIB): $(LIB_OBJS) $(BUILTIN_OBJS)
	$(MKDIR) $(BIN_LIB)
	$(CXX) $(LIB_OBJS) $(BUILTIN_OBJS) -shared -o "$@" 

$(BIN_OBJ)/%.o: %.cpp
	mkdir -p $(dir $@)
	$(CXX) $(CXXFLAGS) $(LIBFLAGS) $(OPTIMIZATION) -c $< -o $@

example: static-exmaple

static-example: EXEC_LINKLIBS += $(STATIC_LIB)
static-example: $(STATIC_LIB) $(EXAMPLES)

dynamic-example: EXEC_LINKFLAGS += -L$(abspath $(BIN_LIB)) -liash
dynamic-example: $(DYNAMIC_LIB) $(EXAMPLES)
	
$(BIN_EXAMPLE)/%: examples/%.cpp
	mkdir -p $(dir $@)
	$(CXX) $(CXXFLAGS) $(OPTIMIZATION) -I ./ $< $(EXEC_LINKLIBS) -o $@ $(EXEC_LINKFLAGS)
	
clean:
	rm -vrf $(BIN_ROOT)
