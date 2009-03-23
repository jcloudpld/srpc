#
# srpc Makefile
#

# Boost library
BOOST_INC=-I/opt/local/include

# GoogleTest library
GTEST_INC=-I/usr/local/include
GTEST_LIB_DIR=-L/usr/local/lib
GTEST_LIB = $(GTEST_LIB_DIR) -lgtest

# precompiled header
PCH_FILE=stdafx.h
GCH=$(PCH_FILE).gch

# check GCC version (stolen from ACE)
ifeq ($(CXX),insure)
  # insure does not pass through the -dumpversion option.
  CXX_FOR_VERSION_TEST = g++
else
  CXX_FOR_VERSION_TEST = $(CXX)
endif

CXX_VERSION := $(shell $(CXX_FOR_VERSION_TEST) -dumpversion)
ifeq (cmd,$(findstring cmd,$(SHELL)))
  CXX_MAJOR_VERSION := $(shell $(CXX_FOR_VERSION_TEST) -dumpversion | sed -e "s/[^0-9\.]//g" | sed -e "s/\..*$$//")
else
  CXX_MAJOR_VERSION := $(shell $(CXX_FOR_VERSION_TEST) -dumpversion | sed -e 's/[^0-9\.]//g' | sed -e 's/\..*$$//')
endif
ifeq ($(findstring $(CXX_MAJOR_VERSION),1 2 3),$(CXX_MAJOR_VERSION))
  GXX_4_OR_BETTER := 0
else
  GXX_4_OR_BETTER := 1
endif

ifeq ($(GXX_4_OR_BETTER), 1)
  GCC_VISIBILITY = -DGCC_HASCLASSVISIBILITY -fvisibility=hidden
  GCC_VISIBILITY += -fvisibility-inlines-hidden
endif # GXX_4_OR_BETTER == 1

# OS specific options
OS_TYPE := $(shell $(CXX_FOR_VERSION_TEST) -dumpmachine)
ifeq (apple-darwin,$(findstring apple-darwin,$(OS_TYPE)))
  CXXFLAGS += -pthread
  SOFLAGS = -dynamiclib -Wl,-undefined -Wl,dynamic_lookup -Wl,-single_module
  SOEXT = dylib
else
  CXXFLAGS = -D_REENTRANT -fPIC
  SOFLAGS = -shared
  SOEXT = so
endif

# compiler options
CXXFLAGS += -Wall -Wpointer-arith -fexceptions
CXXFLAGS += $(BOOST_INC)
CXXFLAGS += -DSRPC_DLL

# debug options
ifdef DEBUG
  CXXFLAGS += -g
else
  CXXFLAGS += -DNDEBUG -O3
endif

.PHONY: all clean
all: $(GCH) $(TARGET)

clean:
	rm -f $(OBJECTS) $(TARGET) $(DEPENDENCIES) $(GCH)
	rm -f core core.*

# generate dependency file
%.d : %.cpp
	$(CXX) -M $(CXXFLAGS) $< > $@.$$$$; \
	sed 's,\($*\)\.o[ :]*,\1.o $@ : ,g' < $@.$$$$ > $@; \
	rm -f $@.$$$$

# generate precompiled header
$(GCH): $(PCH_FILE)
	$(CXX) -c -x c++ $(CXXFLAGS) $(PCH_FILE) -o $@

# compile C++ source file
%.o : %.cpp
	$(CXX) -c $(CXXFLAGS) $< -o $@

