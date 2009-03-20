#
# srpc unit-test Makefile
#

include ../build/gnu/inc.mk

TARGET=../bin/srpctest
ifeq "$(DEBUG)" "yes"
  TARGET := $(TARGET)d
  LIBS += -L../lib -lsrpcd
else
  LIBS += -L../lib -lsrpc
endif

SOURCES=*.cpp
OBJECTS := $(patsubst %.cpp,%.o,$(wildcard $(SOURCES)))
DEPENDENCIES := $(patsubst %.cpp,%.d,$(wildcard $(SOURCES)))

CXXFLAGS += -I./ -I../include/
CXXFLAGS += $(GTEST_INC)
LIBS += $(GTEST_LIB)

.PHONY: all srpc
all: srpc $(TARGET)

srpc:
	cd ../src && make all && cd ../test

$(TARGET): $(OBJECTS)
	$(CXX) $(CXXFLAGS) $(LDFLAGS) -o $(TARGET) *.o $(LIBS)

test: all
	LD_LIBRARY_PATH=../lib $(TARGET)

ifneq "$(MAKECMDGOALS)" "clean"
  -include $(DEPENDENCIES)
endif

