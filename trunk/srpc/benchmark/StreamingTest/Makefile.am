EXTRA_DIST = StreamingTest.vcproj StreamingTest_vc8.vcproj

noinst_PROGRAMS = StreamingTest

StreamingTest_SOURCES = \
	stdafx.h \
	stdafx.cpp \
	StreamingTest.cpp

StreamingTest_CPPFLAGS = $(AM_CPPFLAGS) -DSRPC_DLL
StreamingTest_CXXFLAGS = $(AM_CXXFLAGS) -I$(top_builddir)/include

StreamingTest_LDADD= $(top_builddir)/src/libsrpc.la 
 
