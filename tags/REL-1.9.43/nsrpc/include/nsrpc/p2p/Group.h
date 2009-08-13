#ifndef NSRPC_GROUP_H
#define NSRPC_GROUP_H

#ifdef _MSC_VER
#  pragma once
#endif

#include "PeerId.h"

namespace nsrpc
{

/** @addtogroup p2p
* @{
*/

/// group name RPC type
typedef srpc::RShortString RGroupName;

enum GroupId
{
    giUnknown = -1,

    giFirst = 0,

    maxGroupCount = 10
};


inline bool isValid(GroupId groupId)
{
    return (giFirst <= groupId) && (groupId < maxGroupCount);
}


/// group id's RPC type
typedef srpc::RpcIntType<GroupId, 5 + 1> RGroupId;


/**
 * @struct RGroupInfo
 * P2P group information
 */
struct RGroupInfo
{
    RGroupId groupId_;
    RGroupName groupName_;
    RPeerIds peerIds_; ///< participants

    RGroupInfo(GroupId groupId = giUnknown, const RGroupName& groupName = "") :
        groupId_(groupId),
        groupName_(groupName) {}

    bool join(PeerId peerId) {
        if (isExists(peerId)) {
            return false;
        }
        peerIds_.push_back(peerId);
        return true;
    }

    bool leave(PeerId peerId) {
        if (! isExists(peerId)) {
            return false;
        }
        peerIds_.erase(getPeerPos(peerId));
        return true;
    }

    bool isExists(PeerId peerId) const {
        return getPeerPos(peerId) != peerIds_.end();
    }

    RPeerIds::const_iterator getPeerPos(PeerId peerId) const {
        return std::find(peerIds_.begin(), peerIds_.end(), peerId);
    }

    RPeerIds::iterator getPeerPos(PeerId peerId) {
        return std::find(peerIds_.begin(), peerIds_.end(), peerId);
    }

    template <typename Stream>
    void serialize(Stream& stream) {
        stream & groupId_ & groupName_ & peerIds_;
    }
};


typedef srpc::RMap<RGroupId, RGroupInfo, 5> RGroupMap;

/** @} */ // addtogroup p2p

} // namespace nsrpc

#endif // NSRPC_GROUP_H
