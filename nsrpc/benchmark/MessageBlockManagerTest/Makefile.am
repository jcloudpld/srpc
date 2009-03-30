EXTRA_DIST = MessageBlockManagerTest.vcproj MessageBlockManagerTest_vc8.vcproj

noinst_PROGRAMS = MessageBlockManagerTest

MessageBlockManagerTest_SOURCES = \
	stdafx.h \
	stdafx.cpp \
	MessageBlockManagerTest.cpp

MessageBlockManagerTest_CPPFLAGS = $(AM_CPPFLAGS) -DNSRPC_DLL -DSRPC_DLL
MessageBlockManagerTest_CXXFLAGS = $(AM_CXXFLAGS) -I$(top_builddir)/include  -I$(top_srcdir)/include

MessageBlockManagerTest_LDADD= $(top_builddir)/src/libnsrpc.la 
 
gch_file = stdafx.h.gch
BUILT_SOURCES = $(gch_file)
%.h.gch: %.h
	$(CXXCOMPILE) -c $(MessageBlockManagerTest_CPPFLAGS) $(MessageBlockManagerTest_CXXFLAGS) $<

clean-local:
	-rm -f $(gch_file)
