#ifndef NSRPC_GROUPMANAGER_H
#define NSRPC_GROUPMANAGER_H

#include "nsrpc/p2p/Group.h"
#include <boost/noncopyable.hpp>

namespace nsrpc
{

namespace detail
{

class RpcSystemService;

/** @addtogroup p2p
* @{
*/

/**
 * @class GroupManager
 * P2P Peer Manager
 */
class GroupManager : public boost::noncopyable
{
public:
    GroupManager(RpcSystemService& systemService);
    ~GroupManager();

    GroupId createGroup(const RGroupName& groupName);
    bool joinGroup(GroupId groupId, PeerId peerId);

    void groupCreated(const RGroupInfo& groupInfo);
    void groupJoined(GroupId groupId, PeerId peerId);

    const RGroupMap& getGroups() const {
        return groupMap_;
    }

private:
    GroupId getAvailableGroupId() const;

    bool isExists(GroupId groupId) const {
        return groupMap_.find(groupId) != groupMap_.end();
    }

    /// @pre isExists(groupId)
    const RGroupInfo& getGroup(GroupId groupId) const {
        assert(isExists(groupId));
        return (*groupMap_.find(groupId)).second;
    }

    /// @pre isExists(groupId)
    RGroupInfo& getGroup(GroupId groupId) {
        assert(isExists(groupId));
        return (*groupMap_.find(groupId)).second;
    }

private:
    RpcSystemService& systemService_;
    RGroupMap groupMap_;
};

/** @} */ // addtogroup p2p

} // namespace detail

} // namespace nsrpc

#endif // NSRPC_GROUPMANAGER_H