EXTRA_DIST = NamingService.vcproj NamingService_vc8.vcproj

noinst_PROGRAMS = NamingService

NamingService_SOURCES = \
	stdafx.h \
	stdafx.cpp \
	NamingService.h \
	NamingService.cpp \
	NamingServiceClient.h \
	NamingServiceClient.cpp \
	NamingServiceNetwork.h \
	NamingServiceNetwork.cpp \
	NamingServiceServer.h \
	NamingServiceServer.cpp

NamingService_CPPFLAGS = $(AM_CPPFLAGS) -DSRPC_DLL
NamingService_CXXFLAGS = $(AM_CXXFLAGS) -I$(top_builddir)/include -I$(top_srcdir)/include

NamingService_LDADD= $(top_builddir)/src/libsrpc.la 
