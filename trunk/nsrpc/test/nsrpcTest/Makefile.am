EXTRA_DIST = nsrpcTest.vcproj nsrpcTest_vc8.vcproj

TESTS = nsrpcTest
check_PROGRAMS = $(TESTS)

nsrpcTest_SOURCES = \
	AceTestFixture.h \
	CachedSessionManagerTest.cpp \
	ClientSessionTest.cpp \
	ClientSessionTestFixture.h \
	LogManagerTest.cpp \
	MemoryPoolTest.cpp \
	MessageBlockManagerTest.cpp \
	MessageBlockStreamBufferTest.cpp \
	nsrpcTest.cpp \
	ObjectAllocatorTest.cpp \
	ObjectPoolTest.cpp \
	P2pGroupTest.cpp \
	P2pHolePunchingTest.cpp \
	P2pHostMigrationTest.cpp \
	P2pRelayTest.cpp \
	P2pRelayTestFixture.h \
	P2pSessionTest.cpp \
	P2pSessionTestFixture.cpp \
	P2pSessionTestFixture.h \
	RpcSessionTest.cpp \
	SessionAcceptorTest.cpp \
	SessionConnectorTest.cpp \
	SessionTest.cpp \
	SessionTestFixture.h \
	SmartPtrTest.cpp \
	stdafx.cpp \
	stdafx.h \
	TestCachedSessionManager.h \
	TestClient.h \
	TestClientSession.h \
	TestSession.h \
	TestSessionManager.h \
	VariousMemoryAllocatorTest.cpp

nsrpcTest_CPPFLAGS = $(AM_CPPFLAGS) -DNSRPC_DLL -DSRPC_DLL
nsrpcTest_CXXFLAGS = $(AM_CXXFLAGS) -I$(top_builddir)/include  -I$(top_srcdir)/include

nsrpcTest_LDADD = $(top_builddir)/src/libnsrpc.la 
nsrpcTest_LDFLAGS = -lsrpc -lgtest
 
gch_file = stdafx.h.gch
BUILT_SOURCES = $(gch_file)
%.h.gch: %.h
	$(CXXCOMPILE) -c $(nsrpcTest_CPPFLAGS) $(nsrpcTest_CXXFLAGS) $<

clean-local:
	-rm -f $(gch_file)
