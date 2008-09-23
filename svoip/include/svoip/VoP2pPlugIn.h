#ifndef SVOIP_VOP2PPLUGIN_H
#define SVOIP_VOP2PPLUGIN_H

#include "config/config.h"
#include "RecorderCallback.h"
#include <nsrpc/p2p/RpcPlugIn.h>
#include <nsrpc/p2p/PeerHint.h>
#include <nsrpc/p2p/detail/P2pRpcTypes.h>
#ifdef _MSC_VER
#  pragma warning (push)
#  pragma warning (disable: 4127 4244 4267 4312 4996)
#endif
#include <ace/Thread_Mutex.h>
#ifdef _MSC_VER
#  pragma warning (pop)
#endif
#include <boost/scoped_ptr.hpp>
#include <boost/noncopyable.hpp>

namespace svoip
{

class Recorder;
class Player;

typedef srpc::RpcUIntType<Speech> RSpeech;
typedef srpc::RpcUIntType<Sequence> RSequence;

/**
 * @class RpcVoP2pService
 */
class RpcVoP2pService
{
public:
    virtual ~RpcVoP2pService() {}

    DECLARE_SRPC_PURE_METHOD_4(RpcVoP2pService, say,
        nsrpc::detail::RMessageBuffer, samples, srpc::RUInt8, frames,
        RSpeech, speech, RSequence, sequence);
};


/**
 * @class VoP2pPlugIn
 * Voice Over P2P Plug-in
 */
class SVOIP_API VoP2pPlugIn : public nsrpc::RpcPlugIn,
    public svoip::RecorderCallback,
    public RpcVoP2pService
{
    struct SampleChunk {
        nsrpc::PeerHint hint_;
        nsrpc::detail::RMessageBuffer buffer_;
        size_t frames_;
        Speech speech_;
        Sequence sequence_;

        SampleChunk(const nsrpc::PeerHint& hint,
            EncodedSample* sample, size_t sampleLen, size_t frames,
            Speech speech, Sequence sequence) :
            hint_(hint),
            buffer_(sample, sampleLen),
            frames_(frames),
            speech_(speech),
            sequence_(sequence) {}
    };

    typedef std::queue<SampleChunk> SampleQueue;
public:
    VoP2pPlugIn();
    virtual ~VoP2pPlugIn();

    /// broadcast
    void record();

    /// unicast
    void record(nsrpc::PeerId to);

    /// multicast
    void record(nsrpc::GroupId to);

    void stop();

private:
    virtual bool initialize();
    virtual void update();

    virtual void onPeerConnected(nsrpc::PeerId peerId);
    virtual void onPeerDisconnected(nsrpc::PeerId peerId);

private:
    virtual std::auto_ptr<Recorder> createRecorder() = 0;
    virtual std::auto_ptr<Player> createPlayer() = 0;

private:
    // = svoip::RecorderCallback overriding
    virtual void sampled(nsrpc::PeerId targetPeerId,
        nsrpc::GroupId targetGroupId, EncodedSample* sample, size_t sampleLen,
        size_t frames, Speech speech, Sequence sequence);

public:
    // = RpcVoP2pService overriding
    DECLARE_SRPC_P2P_METHOD_4(say,
        nsrpc::detail::RMessageBuffer, samples, srpc::RUInt8, frames,
        RSpeech, speech, RSequence, sequence);

private:
    boost::scoped_ptr<Recorder> recorder_;
    boost::scoped_ptr<Player> player_;

    SampleQueue sampleQueue_;
    ACE_Thread_Mutex lock_;
};

} // namespace svoip

#endif // SVOIP_VOP2PPLUGIN_H
