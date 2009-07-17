EXTRA_DIST = EchoTest.vcproj EchoTest_vc8.vcproj

noinst_PROGRAMS = EchoTest

EchoTest_SOURCES = \
	Config.cpp \
	Config.h \
	EchoClient.cpp \
	EchoClient.h \
	EchoServer.cpp \
	EchoServer.h \
	EchoTest.cpp \
	stdafx.h \
	stdafx.cpp

EchoTest_CPPFLAGS = $(AM_CPPFLAGS) -DNSRPC_DLL -DSRPC_DLL
EchoTest_CXXFLAGS = $(AM_CXXFLAGS) -I$(top_builddir)/include  -I$(top_srcdir)/include

EchoTest_LDADD = $(top_builddir)/src/libnsrpc.la
EchoTest_LIBS = -lsrpc
