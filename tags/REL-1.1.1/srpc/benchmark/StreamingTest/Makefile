#
# StreamingTest Makefile
#

include ../../build/gnu/inc.mk

TARGET=../../bin/StreamingTest
ifeq "$(DEBUG)" "yes"
  TARGET := $(TARGET)d
  LIBS += -L../../lib -lsrpcd
else
  LIBS += -L../../lib -lsrpc
endif

SOURCES=*.cpp
OBJECTS := $(patsubst %.cpp,%.o,$(wildcard $(SOURCES)))
DEPENDENCIES := $(patsubst %.cpp,%.d,$(wildcard $(SOURCES)))

CXXFLAGS += -I./ -I../../include/

.PHONY: all srpc
all: srpc $(TARGET)

srpc:
	cd ../../src && make all && cd ../benchmark/StreamingTest

$(TARGET): $(OBJECTS)
	$(CXX) $(CXXFLAGS) $(LDFLAGS) -o $(TARGET) *.o $(LIBS)

run: $(TARGET)
	LD_LIBRARY_PATH=../../lib $(TARGET)

ifneq "$(MAKECMDGOALS)" "clean"
  -include $(DEPENDENCIES)
endif

