EXTRA_DIST = SimpleRelayServer.vcproj SimpleRelayServer_vc8.vcproj

noinst_PROGRAMS = SimpleRelayServer

SimpleRelayServer_SOURCES = \
	SimpleRelayServer.cpp \
	stdafx.cpp \
	stdafx.h

SimpleRelayServer_CPPFLAGS = $(AM_CPPFLAGS) -DNSRPC_DLL -DSRPC_DLL
SimpleRelayServer_CXXFLAGS = $(AM_CXXFLAGS) -I$(top_builddir)/include  -I$(top_srcdir)/include

SimpleRelayServer_LDADD= $(top_builddir)/src/libnsrpc.la 
