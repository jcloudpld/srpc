EXTRA_DIST = srpcTest.vcproj srpcTest_vc8.vcproj

TESTS = srpcTest
check_PROGRAMS = $(TESTS)

srpcTest_SOURCES = \
	DummyRpcCommand.h \
	DummyRpcEvent.h \
	DummyRpcInterface.h \
	DummyStreamBuffer.h \
	StreamTexture.h \
	DummyRpcClient.h \
	DummyRpcClient.cpp \
	DummyRpcServer.h \
	DummyRpcServer.cpp \
	ForwardingFunctorTest.cpp \
	IBitStreamTest.cpp \
	IByteStreamTest.cpp \
	MockRpcNetwork.h \
	MockRpcNetwork.cpp \
	MultipleRpcReceiverTest.cpp \
	OBitStreamTest.cpp \
	OByteStreamTest.cpp \
	ReceivingFunctorTest.cpp \
	RpcCommandTest.cpp \
	RpcConainersTest.cpp \
	RpcEventTest.cpp \
	RpcForwarderTest.cpp \
	RpcIdTest.cpp \
	RpcReceiverTest.cpp \
	RpcTypesTest.cpp \
	srpcTest.cpp \
	stdafx.h \
	stdafx.cpp \
	UnicodeTest.cpp \
	VectorStreamBufferTest.cpp

srpcTest_CPPFLAGS = $(AM_CPPFLAGS) -DSRPC_DLL
srpcTest_CXXFLAGS = $(AM_CXXFLAGS) -I$(top_builddir)/include -I$(top_srcdir)/include

srpcTest_LDADD = $(top_builddir)/src/libsrpc.la 
srpcTest_LDFLAGS = -lgtest
