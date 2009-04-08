#ifndef NSRPC_PEERCANDIDATE_H
#define NSRPC_PEERCANDIDATE_H

#include "P2pMessages.h"
#include <nsrpc/p2p/detail/P2pAddress.h>
#include <nsrpc/utility/SmartPtr.h>
#include <srpc/ContainerTypes.h>

namespace nsrpc
{

namespace detail
{

class PeerNetworkSender;
class PeerCandidate;

/** @addtogroup p2p
* @{
*/

/**
 * @class PeerCandidateManager
 * P2P 접속을 위해 임시 Peer를 관리한다.
 * - 임시 Peer가 접속이 완료되면 삭제되고 PeerManager에 등록된다.
 */
class PeerCandidateManager
{
    typedef SmartPtr<PeerCandidate> PeerCandidatePtr;
    typedef srpc::Vector<PeerCandidatePtr> PeerCandidates;
public:
    PeerCandidateManager(PeerNetworkSender& networkSender,
        const P2pConfig& p2pConfig, PeerId myPeerId);
    ~PeerCandidateManager();

    void reset();

    void addCandidate(PeerId peerId, const AddressPair& addressPair);
    void removeCandidates(PeerId peerId);

    bool putOutgoingMessage(PeerId peerId, const ACE_INET_Addr& targetAddress,
        ACE_Message_Block* mblock);

    PeerIds detectConnectionTimeout();

    size_t getCandidateCount() const {
        return candidates_.size();
    }

    bool isExists(PeerId peerId, const ACE_INET_Addr& targetAddress) const {
        return isExists(getCandidatePos(peerId, targetAddress));
    }
private:
    bool isExists(PeerId peerId) const;

    bool isExists(PeerCandidates::iterator pos) const {
        return pos != candidates_.end();
    }

    bool isExists(PeerCandidates::const_iterator pos) const {
        return pos != candidates_.end();
    }

    PeerCandidates::const_iterator getCandidatePos(PeerId peerId,
        const ACE_INET_Addr& targetAddress) const;

    PeerCandidates::iterator getCandidatePos(PeerId peerId,
        const ACE_INET_Addr& targetAddress);
private:
    PeerNetworkSender& networkSender_;
    const P2pConfig& p2pConfig_;
    const PeerId myPeerId_;
    PeerCandidates candidates_;
};

/** @} */ // addtogroup p2p

} // namespace detail

} // namespace nsrpc

#endif // NSRPC_PEERCANDIDATE_H
