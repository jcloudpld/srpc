#ifndef NSRPC_PEERCIPHERKEYCACHE_H
#define NSRPC_PEERCIPHERKEYCACHE_H

#include <nsrpc/p2p/PeerId.h>
#include <nsrpc/detail/PacketCoder.h>
#ifdef _MSC_VER
#  pragma warning (push)
#  pragma warning (disable: 4702)
#endif
#include <hash_map>
#ifdef _MSC_VER
#  pragma warning (pop)
#endif
#include <string>
#include <sstream>

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
 * - ���� �ð��� �����̶� ���Ϸ��� ����. ^^;
 */
class PeerCipherKeyCache: public boost::noncopyable
{
    typedef stdext::hash_map<PeerId, std::string> CipherKeyMap;
public:
    PeerCipherKeyCache(const PacketCoder& packetCoder) :
        packetCoder_(packetCoder) {}

    const std::string& get(PeerId peerId) const {
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

    void set(PeerId peerId, const std::string& cipherKey) {
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
