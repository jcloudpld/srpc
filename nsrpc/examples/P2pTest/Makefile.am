EXTRA_DIST = P2pTest.vcproj P2pTest_vc8.vcproj

noinst_PROGRAMS = P2pTest

P2pTest_SOURCES = \
	Config.cpp \
	Config.h \
	P2pTest.cpp \
	Peer.cpp \
	Peer.h \
	PeerService.h \
	stdafx.cpp \
	stdafx.h

P2pTest_CPPFLAGS = $(AM_CPPFLAGS) -DNSRPC_DLL -DSRPC_DLL
P2pTest_CXXFLAGS = $(AM_CXXFLAGS) -I$(top_builddir)/include  -I$(top_srcdir)/include

P2pTest_LDADD= $(top_builddir)/src/libnsrpc.la
