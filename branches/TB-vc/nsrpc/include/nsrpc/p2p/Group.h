#ifndef NSRPC_GROUP_H
#define NSRPC_GROUP_H

#include <srpc/RpcTypes.h>
#include <srpc/RpcContainers.h>

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

    RGroupInfo(GroupId groupId = giUnknown, const RGroupName& groupName = "") :
        groupId_(groupId),
        groupName_(groupName) {}

    void write(srpc::OStream& os) {
        groupId_.write(os);
        groupName_.write(os);

    }

    void read(srpc::IStream& is) {
        groupId_.read(is);
        groupName_.read(is);

    }
};


typedef srpc::RMap<RGroupId, RGroupInfo, 5> RGroupMap;

/** @} */ // addtogroup p2p

} // namespace nsrpc

#endif // NSRPC_GROUP_H
