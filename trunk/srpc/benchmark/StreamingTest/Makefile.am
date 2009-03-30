EXTRA_DIST = StreamingTest.vcproj StreamingTest_vc8.vcproj

noinst_PROGRAMS = StreamingTest

StreamingTest_SOURCES = \
	stdafx.h \
	stdafx.cpp \
	StreamingTest.cpp

StreamingTest_CPPFLAGS = $(AM_CPPFLAGS) -DSRPC_DLL
StreamingTest_CXXFLAGS = $(AM_CXXFLAGS) -I$(top_builddir)/include -I$(top_srcdir)/include

StreamingTest_LDADD= $(top_builddir)/src/libsrpc.la 
 
gch_file = stdafx.h.gch
BUILT_SOURCES = $(gch_file)
%.h.gch: %.h
	$(CXXCOMPILE) -c $(StreamingTest_CPPFLAGS) $(StreamingTest_CXXFLAGS) $<

clean-local:
	-rm -f $(gch_file)
