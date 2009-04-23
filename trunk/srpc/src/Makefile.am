EXTRA_DIST = srpc.vcproj srpc_vc8.vcproj

lib_LTLIBRARIES = libsrpc.la

libsrpc_la_SOURCES = \
	IBitStream.cpp \
 	IByteStream.cpp \
	OBitStream.cpp \
	OByteStream.cpp \
	RpcCommand.cpp \
	RpcForwarder.cpp \
	RpcNetwork.cpp \
	RpcReceiver.cpp \
	stdafx.h \
	stdafx.cpp \
	StreamFactory.cpp \
	Unicode.cpp

libsrpc_la_CPPFLAGS = $(AM_CPPFLAGS) -DSRPC_DLL -DSRPC_EXPORTS
libsrpc_la_CXXFLAGS = $(AM_CXXFLAGS) -I$(top_builddir)/include -I$(top_srcdir)/include

libsrpc_la_LDFLAGS= -no-undefined
