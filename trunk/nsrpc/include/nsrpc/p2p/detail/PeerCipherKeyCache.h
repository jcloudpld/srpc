#ifndef NSRPC_PEERCIPHERKEYCACHE_H
#define NSRPC_PEERCIPHERKEYCACHE_H

#include <nsrpc/p2p/PeerId.h>
#include <nsrpc/detail/PacketCoder.h>
#include <srpc/ContainerTypes.h>

namespace nsrpc
{

namespace detail
{

/** @addtogroup p2p
* @{
*/

/**
 * @class PeerCipherKeyCache
 * PeerId -> String table
 * - 실행 시간을 조금이라도 줄일려고 만듦. ^^;
 */
class PeerCipherKeyCache: public boost::noncopyable
{
    typedef srpc::HashMap<PeerId, srpc::String> CipherKeyMap;
public:
    PeerCipherKeyCache(const PacketCoder& packetCoder) :
        packetCoder_(packetCoder) {}

    const srpc::String& get(PeerId peerId) const {
        CipherKeyMap::const_iterator pos = cipherKeys_.find(peerId);
        if (pos == cipherKeys_.end()) {
            const PacketCoder::Seed seed =
                packetCoder_.generateCipherSeed(peerId);
            cipherKeys_.insert(CipherKeyMap::value_type(peerId, seed));
            pos = cipherKeys_.find(peerId);
        }
        assert(pos != cipherKeys_.end());
        return (*pos).second;
    }

    void set(PeerId peerId, const srpc::String& cipherKey) {
        CipherKeyMap::iterator pos = cipherKeys_.find(peerId);
        if (pos != cipherKeys_.end()) {
            (*pos).second = cipherKey;
        }
        else {
            cipherKeys_.insert(CipherKeyMap::value_type(peerId, cipherKey));
        }
    }
private:
    const PacketCoder& packetCoder_;
    mutable CipherKeyMap cipherKeys_;
};

/** @} */ // addtogroup p2p

} // namespace detail

} // namespace nsrpc

#endif // NSRPC_PEERCIPHERKEYCACHE_H
