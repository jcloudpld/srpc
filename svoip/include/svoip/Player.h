#ifndef SVOIP_PLAYER_H
#define SVOIP_PLAYER_H

#include "config/config.h"
#include <nsrpc/p2p/PeerId.h>
#include <nsrpc/utility/SmartPtr.h>
#include <boost/scoped_ptr.hpp>
#include <boost/noncopyable.hpp>

namespace svoip
{

class Decoder;

namespace detail
{
class PlayerTask;
} // namespace detail


/**
 * @class Player
 * Audio decode & play
 */
class SVOIP_API Player : public boost::noncopyable
{
    typedef nsrpc::SmartPtr<Decoder> DecoderPtr;
    typedef srpc::Map<nsrpc::PeerId, DecoderPtr> DecoderMap;

    friend class detail::PlayerTask;
public:
    Player();
    virtual ~Player();

    virtual bool open();

    virtual void close();

    virtual void start() = 0;

    virtual void stop() = 0;

private:
    virtual void playNow(const svoip::Sample* sample, size_t samples) = 0;

    virtual bool run() = 0;

public:
    void play(nsrpc::PeerId fromPeerId,
        const EncodedSample* sample, size_t samples, size_t frames,
        Speech speech, Sequence sequence);

    void addDecoder(nsrpc::PeerId peerId);
    void removeDecoder(nsrpc::PeerId peerId);

private:
    DecoderPtr getDecoder(nsrpc::PeerId peerId);

private:
    DecoderMap decoderMap_;
    boost::scoped_ptr<detail::PlayerTask> task_;
};

} // namespace svoip

#endif // SVOIP_PLAYER_H
