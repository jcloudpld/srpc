#
# nsrpc Makefile
#

include ../build/gnu/inc.mk

TARGET := ../lib/libnsrpc
ifeq "$(DEBUG)" "yes"
  TARGET := $(TARGET)d
endif
TARGET := $(TARGET).so

SOURCES := *.cpp
OBJECTS := $(patsubst %.cpp,%.o,$(wildcard $(SOURCES)))
DEPENDENCIES := $(patsubst %.cpp,%.d,$(wildcard $(SOURCES)))

CXXFLAGS += $(GCC_VISIBILITY) -DNSRPC_EXPORTS
CXXFLAGS += -I./ -I../include/
CXXFLAGS += -shared
LIBS += $(SRPC_LIB) $(TPROACTOR_LIB) $(ACE_LIB)

all: $(TARGET)
test: all

$(TARGET): $(OBJECTS)
	$(CXX) $(CXXFLAGS) $(LDFLAGS) -o $(TARGET) *.o $(LIBS)

ifneq "$(MAKECMDGOALS)" "clean"	
  -include $(DEPENDENCIES)
endif

