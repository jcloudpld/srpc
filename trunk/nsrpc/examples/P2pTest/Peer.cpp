#include "stdafx.h"
#include "Peer.h"
#include "Config.h"
#include <nsrpc/p2p/P2pSessionFactory.h>
#include <nsrpc/p2p/PeerHint.h>
#include <nsrpc/utility/SystemUtil.h>
#include <sstream>
#include <windows.h>

namespace
{

srpc::String toString(const nsrpc::PeerAddresses& addresses)
{
    srpc::OStringStream oss;

    for (size_t i = 0; i < addresses.size(); ++i) {
        if (i != 0) {
            oss << ",";
        }
        oss << addresses[i].ip_ << ":" << addresses[i].port_;
    }
    return oss.str();
}

} // namespace

IMPLEMENT_SRPC_EVENT_DISPATCHER(Peer);

#ifdef _MSC_VER
#  pragma warning (push)
#  pragma warning (disable: 4355)
#endif

Peer::Peer(const Config& config) :
    config_(config),
    isResolved_(false),
    connectFailedPeerId_(nsrpc::invalidPeerId),
    tickCount_(0),
    printedTime_(0),
    lastSentTime_(0)
{
    nsrpc::P2pConfig p2pConfig;
    p2pConfig.sendPacketLossRate_ = config.getSendPacketLossRate();
    p2pConfig.recvPacketLossRate_ = config.getRecvPacketLossRate();

    p2pSession_.reset(nsrpc::P2pSessionFactory::create(config_.getPeerId(),
        *this, p2pConfig));

    srpc::RpcReceiver::setRpcNetwork(&p2pSession_->getRpcNetwork());
    srpc::RpcForwarder::setRpcNetwork(&p2pSession_->getRpcNetwork());
}

#ifdef _MSC_VER
#  pragma warning (pop)
#endif


Peer::~Peer()
{
    p2pSession_->close();
}


bool Peer::ready()
{
    const nsrpc::PeerTime resolvingTimeout = 2000;

    if (! p2pSession_->open(config_.getListeningPort())) {
        std::cout << "* Failed to open P2pSession.\n";
        return false;
    }

    if (config_.getRelayServerAddress().isValid()) {
        std::cout << "* Resolving the address...\n";
        const nsrpc::PeerTime currentTime = nsrpc::detail::getPeerTime();
        p2pSession_->setRelayServer(config_.getRelayServerAddress());
        while (! isResolved_) {
            p2pSession_->tick();
            if ((nsrpc::detail::getPeerTime() - currentTime) >=
                resolvingTimeout) {
                std::cout << "* Failed to resolve the address.\n";
                break;
            }
            //nsrpc::pause(1);
        }
    }

    if (config_.isHost()) {
        p2pSession_->host();
        std::cout << "* I'm a P2P host. Waiting for other peer(s)...\n";
    }
    else {
        p2pSession_->connect(config_.getHostAddresses());
    }

    printedTime_ = nsrpc::detail::getPeerTime();

    return true;
}


bool Peer::tick()
{
    p2pSession_->tick();

    if (! config_.isHost()) {
        if (connectFailedPeerId_ == nsrpc::pseudoHostPeerId) {
            return false;
        }
    }

    if (joiners_.size() >= 1) {
        if ((nsrpc::detail::getPeerTime() - lastSentTime_) >= tickInterval) {
            if ((++tickCount_) >= INT_MAX) {
                return false;
            }
            tick(tickCount_);
            lastSentTime_ = nsrpc::detail::getPeerTime();
        }

        if ((nsrpc::detail::getPeerTime() - printedTime_) >= printInterval) {
            printAllStats();
            printedTime_ = nsrpc::detail::getPeerTime();
        }
    }

    nsrpc::pause(1);

    return true;
}


void Peer::printAllStats()
{
    PeerIdSet::const_iterator pos = joiners_.begin();
    const PeerIdSet::const_iterator end = joiners_.end();
    for (; pos != end; ++pos) {
        if (*pos != p2pSession_->getPeerId()) {
            printStats(*pos);
        }
    }
}


void Peer::printStats(nsrpc::PeerId peerId)
{
    const nsrpc::PeerAddress address = p2pSession_->getTargetAddress(peerId);
    const nsrpc::PeerStats stats = p2pSession_->getStats(peerId);
    std::cout << "* Peer(P" << peerId << ") Stats:\n" <<
        "  TargetAddress=" << address.ip_ << ":" << address.port_ << ",\n" <<
        "  PacketsSentReliable=" << stats.packetsSentReliable_ << ",\n" <<
        "  PacketsSentUnreliable=" << stats.packetsSentUnreliable_ << ",\n" <<
        "  PacketsReceivedReliable=" << stats.packetsReceivedReliable_ <<
        ",\n" <<
        "  PacketsReceivedUnreliable=" << stats.packetsReceivedUnreliable_ <<
        ",\n" <<
        "  PacketsLost=" << stats.packetsLost_ << ",\n" <<
        "  MeanRoundTripTime=" << stats.meanRoundTripTime_ << ",\n" <<
        "  HighestRoundTripTime=" << stats.highestRoundTripTime_ << ",\n" <<
        "  RoundTripTimeVariance=" << stats.roundTripTimeVariance_ << ",\n" <<
        "  HighestRoundTripTimeVariance=" <<
        stats.highestRoundTripTimeVariance_ << "\n";
}

// = P2pEventHandler overring

void Peer::onPeerConnected(nsrpc::PeerId peerId)
{
    const nsrpc::PeerAddress targetAddress =
        p2pSession_->getTargetAddress(peerId);
    std::cout << "* Peer(P" << peerId << ", " <<
        targetAddress.ip_ << ":" << targetAddress.port_ <<
        ", [" << toString(p2pSession_->getAddresses(peerId)) <<
        "]) connected.\n";

    srpc::OStringStream oss;
    oss << "Hi~ P" << peerId;
    const nsrpc::PeerHint hint(peerId);
    chat(oss.str(), &hint); // unicast

    if (! isJoiner(peerId)) {
        joiners_.insert(peerId);
    }
}


void Peer::onPeerDisconnected(nsrpc::PeerId peerId)
{
    std::cout << "* Peer(P" << peerId << ") disconnected.\n";

    srpc::OStringStream oss;
    oss << "Bye~ P" << peerId;
    chat(oss.str());

    printStats(peerId);

    const PeerIdSet::iterator pos = joiners_.find(peerId);
    if (pos != joiners_.end()) {
        joiners_.erase(pos);
    }
}


void Peer::onConnectFailed(nsrpc::PeerId peerId)
{
    std::cout << "* Failed to connect to Peer(P" << peerId << ").\n";
    connectFailedPeerId_ = peerId;
}


void Peer::onAddressResolved(const srpc::String& ipAddress, srpc::UInt16 port)
{
    std::cout << "* Address resolved(" << ipAddress << ":" << port <<
        ", [" << toString(p2pSession_->getAddresses(config_.getPeerId())) <<
        "])\n";

    isResolved_ = true;
}


void Peer::onHostMigrated(nsrpc::PeerId peerId)
{
    std::cout << "* Host Migrated(P" << peerId << ").\n";
}

// = RpcPeerService overriding

IMPLEMENT_SRPC_P2P_METHOD_1(Peer, chat, srpc::RShortString, message,
    srpc::ptReliable)
{
    // rpcHint is a hidden variable. see IMPLEMENT_SRPC_P2P_METHOD_1 macro
    const nsrpc::PeerHint& hint = nsrpc::toPeerHint(rpcHint);

    std::cout << "* Chat(from:P" << hint.peerId_ << "): [" << message << "]\n";

    //const nsrpc::PeerHint toHint(hint.peerId_);
    //dummy(&toHint);
}


IMPLEMENT_SRPC_P2P_METHOD_1(Peer, tick, srpc::RUInt32, tick, srpc::ptReliable)
{
    const nsrpc::PeerHint& hint = nsrpc::toPeerHint(rpcHint);

    if (! isJoiner(hint.peerId_)) {
        return;
    }

    if (hint.peerId_ != p2pSession_->getPeerId()) {
        std::cout << "* Tick(P" << hint.peerId_ << "->P" <<
            p2pSession_->getPeerId() << "): [" << tick << "]\n";
    }

    if (tickMap_.find(hint.peerId_) != tickMap_.end()) {
        const srpc::UInt32 expectedTick = (tickMap_[hint.peerId_] + 1);
        if (tick != expectedTick) {
            std::cout << "* P" << hint.peerId_ << ": " << expectedTick <<
                " expected, but " << tick << " arrived.\n";
            printAllStats();
            return; //exit(1);
        }
    }

    tickMap_[hint.peerId_] = tick;

#if tickProcessingTime > 0
    nsrpc::pause(tickProcessingTime);
#endif
}


IMPLEMENT_SRPC_P2P_METHOD_0(Peer, dummy, srpc::ptUnreliable)
{
    const nsrpc::PeerHint& hint = nsrpc::toPeerHint(rpcHint);

    if (! isJoiner(hint.peerId_)) {
        return;
    }

    std::cout << "* dummy(from:P" << hint.peerId_ << ")\n";
}
