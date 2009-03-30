EXTRA_DIST = nsrpc.vcproj nsrpc_vc8.vcproj

lib_LTLIBRARIES = libnsrpc.la

libnsrpc_la_SOURCES = \
	AceUtil.cpp \
	BandwidthLimit.h \
	BandwidthLimit.cpp \
	CachedSessionManager.cpp \
	ClientSession.cpp \
	CsPacketCoder.h \
	LogManager.cpp \
	PacketCoder.cpp \
	PacketCoderFactory.cpp \
	PacketSeedExchanger.cpp \
	PacketSeedExchangerFactory.cpp \
	PacketSeedExchangerImpl.h \
	PacketSeedExchangerImpl.cpp \
	ProactorFactory.cpp \
	ProactorTask.cpp \
	ReactorTask.cpp \
	RpcClientSession.cpp \
	RpcNetworkForBroadcast.cpp \
	RpcSession.cpp \
	Session.cpp \
	SessionAcceptor.cpp \
	SessionConnector.cpp \
	SessionRpcNetwork.cpp \
	SmartPtr.cpp \
	stdafx.h \
	stdafx.cpp \
	SystemUtil.cpp \
	p2p/AnonymousMessageManager.h \
	p2p/AnonymousMessageManager.cpp \
	p2p/GroupManager.h \
	p2p/GroupManager.cpp \
	p2p/P2pEndpoint.cpp \
	p2p/P2pMessages.h \
	p2p/P2pPacketCoder.h  \
	p2p/P2pProtocol.cpp \
	p2p/P2pRpcNetwork.cpp \
	p2p/P2pSessionFactory.cpp \
	p2p/P2pSessionImpl.h \
	p2p/P2pSessionImpl.cpp \
	p2p/Peer.h \
	p2p/Peer.cpp \
	p2p/PeerCandidate.h \
	p2p/PeerCandidate.cpp \
	p2p/PeerManager.h \
	p2p/PeerManager.cpp \
	p2p/PeerMessageHandler.h \
	p2p/PeerTime.cpp \
	p2p/PlugInManager.h \
	p2p/PlugInManager.cpp \
	p2p/RelayServiceHandler.h \
	p2p/RelayServiceImpl.h \
	p2p/RelayServiceImpl.cpp \
	p2p/RpcPlugIn.cpp \
	p2p/SimpleRelayService.cpp \
	p2p/StunServiceHandler.h \
	p2p/SystemService.h \
	p2p/SystemServiceHandler.h \
	p2p/StunServiceImpl.h \
	p2p/StunServiceImpl.cpp \
	p2p/SystemServiceImpl.h \
	p2p/SystemServiceImpl.cpp

libnsrpc_la_CPPFLAGS = $(AM_CPPFLAGS) -DNSRPC_DLL -DNSRPC_EXPORTS -DSRPC_DLL
libnsrpc_la_CXXFLAGS = $(AM_CXXFLAGS) -I$(top_builddir)/include -I$(top_srcdir)/include

libnsrpc_la_LDFLAGS= \
	-lsrpc \
	-lACE \
	-no-undefined

gch_file = stdafx.h.gch
BUILT_SOURCES = $(gch_file)
%.h.gch: %.h
	$(CXXCOMPILE) -c $(libnsrpc_la_CPPFLAGS) $(libnsrpc_la_CXXFLAGS) $<

clean-local:
	-rm -f $(gch_file)
