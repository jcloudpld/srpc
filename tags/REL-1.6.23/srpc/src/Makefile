#
# srpc Makefile
#

include ../build/gnu/inc.mk

TARGET := ../lib/libsrpc
ifeq "$(DEBUG)" "yes"
  TARGET := $(TARGET)d
endif
TARGET := $(TARGET).so

SOURCES := *.cpp
OBJECTS := $(patsubst %.cpp,%.o,$(wildcard $(SOURCES)))
DEPENDENCIES := $(patsubst %.cpp,%.d,$(wildcard $(SOURCES)))

CXXFLAGS += -DSRPC_EXPORTS $(GCC_VISIBILITY)
CXXFLAGS += -I./ -I../include/
CXXFLAGS += -shared

all: $(TARGET)
test: all

$(TARGET): $(OBJECTS)
	$(CXX) $(CXXFLAGS) $(LDFLAGS) -o $(TARGET) *.o $(LIBS)

ifneq "$(MAKECMDGOALS)" "clean"	
  -include $(DEPENDENCIES)
endif

