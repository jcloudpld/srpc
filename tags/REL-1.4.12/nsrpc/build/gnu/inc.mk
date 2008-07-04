#
# nsrpc Makefile
#

DEBUG=yes
#AIO_ROOT=
#ACE_ROOT=

# srpc library
SRPC_ROOT=/usr/local/srpc
SRPC_INC=-I$(SRPC_ROOT)/include
SRPC_LIB_DIR=$(SRPC_ROOT)/lib
SRPC_LIB=-L$(SRPC_LIB_DIR)
ifeq "$(DEBUG)" "yes"
	SRPC_LIB += -lsrpcd
else
	SRPC_LIB += -lsrpc
endif
LD_LIBRARY_PATH := $(LD_LIBRARY_PATH):$(SRPC_LIB_DIR)

# Boost library
BOOST_INC=-I/usr/include/boost

# CppUnit library
CPPUNIT_INC=-I/usr/include/cppunit
CPPUNIT_LIB_DIR=/usr/lib
CPPUNIT_LIB = -L$(CPPUNIT_LIB_DIR) -lcppunit
#LD_LIBRARY_PATH := $(LD_LIBRARY_PATH):$(CPPUNIT_LIB_DIR)

# TProactor library
TPROACTOR_INC=-I$(AIO_ROOT)
TPROACTOR_LIB_DIR=$(AIO_ROOT)/lib
TPROACTOR_LIB = -L$(TPROACTOR_LIB_DIR) -lTProactor
#LD_LIBRARY_PATH := $(LD_LIBRARY_PATH):$(TPROACTOR_LIB_DIR)

# ACE library
ACE_INC=-I$(ACE_ROOT)
ACE_LIB_DIR=$(ACE_ROOT)/lib
ACE_LIB = -L$(ACE_LIB_DIR) -lACE
#LD_LIBRARY_PATH := $(LD_LIBRARY_PATH):$(ACE_LIB_DIR)

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

# compiler options
CXXFLAGS += -Wall -Wpointer-arith -fexceptions -D_REENTRANT $(SRPC_INC) $(BOOST_INC) $(TPROACTOR_INC) $(ACE_INC) 
CXXFLAGS += -DNSRPC_DLL

# debug options
ifdef DEBUG
	CXXFLAGS += -g
else
	CXXFLAGS += -DNDEBUG -O3
endif

.PHONY: all clean
all: $(GCH) $(TARGET)

clean:
	rm -f $(OBJECTS) $(TARGET) $(GCH) $(DEPENDENCIES)
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

