#
# MessageBlockManagerTest Makefile
#

NSRPC_ROOT=../..

include $(NSRPC_ROOT)/build/gnu/inc.mk

TARGET=$(NSRPC_ROOT)/bin/MessageBlockManagerTest
LIBS += -L$(NSRPC_ROOT)/lib
ifeq "$(DEBUG)" "yes"
  TARGET := $(TARGET)d
  LIBS += -lnsrpcd
else
  LIBS += -lnsrpc
endif

SOURCES=*.cpp
OBJECTS := $(patsubst %.cpp,%.o,$(wildcard $(SOURCES)))
DEPENDENCIES := $(patsubst %.cpp,%.d,$(wildcard $(SOURCES)))

CXXFLAGS += -I./ -I$(NSRPC_ROOT)/include/
LIBS += $(ACE_LIB)

.PHONY: all nsrpc
all: nsrpc $(TARGET)

nsrpc:
	cd ../../src && $(MAKE) all && cd ../test/MessageBlockManagerTest

$(TARGET): $(OBJECTS) $(DEPENDENCIES)
	echo $(DUMMY)
	$(CXX) $(CXXFLAGS) $(LDFLAGS) -o $(TARGET) *.o $(LIBS)

test: all
	LD_LIBRARY_PATH=$(LD_LIBRARY_PATH):$(NSRPC_ROOT)/lib $(TARGET)

ifneq "$(MAKECMDGOALS)" "clean"
  -include $(DEPENDENCIES)
endif
