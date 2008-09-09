#ifndef NSRPC_GROUPMANAGER_H
#define NSRPC_GROUPMANAGER_H

#include "nsrpc/p2p/Group.h"
#include <boost/noncopyable.hpp>

namespace nsrpc
{

namespace detail
{

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
    GroupManager();
    ~GroupManager();

    GroupId createGroup(const RGroupName& groupName);

    void addGroup(const RGroupInfo& groupInfo);

    const RGroupMap& getGroups() const {
        return groupMap_;
    }

    /// @pre isExists(groupId)
    const RGroupInfo& getGroup(GroupId groupId) const {
        assert(isExists(groupId));
        return (*groupMap_.find(groupId)).second;
    }

private:
    GroupId getAvailableGroupId() const;
    bool isExists(GroupId groupId) const {
        return groupMap_.find(groupId) != groupMap_.end();
    }

private:
    RGroupMap groupMap_;
};

/** @} */ // addtogroup p2p

} // namespace detail

} // namespace nsrpc

#endif // NSRPC_GROUPMANAGER_H
