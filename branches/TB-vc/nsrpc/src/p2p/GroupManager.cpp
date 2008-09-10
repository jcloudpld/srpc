#include "stdafx.h"
#include "GroupManager.h"
#include "SystemService.h"

namespace nsrpc
{

namespace detail
{

GroupManager::GroupManager(RpcSystemService& systemService) :
    systemService_(systemService)
{
}



GroupManager::~GroupManager()
{
}


GroupId GroupManager::createGroup(const RGroupName& groupName)
{
    if (groupMap_.size() >= maxGroupCount) {
        return giUnknown;
    }

    const GroupId groupId = getAvailableGroupId();
    if (! isValid(groupId)) {
        return giUnknown;
    }

    const RGroupInfo groupInfo(groupId, groupName);
    groupMap_.insert(RGroupMap::value_type(groupId, groupInfo));

    systemService_.rpcGroupCreated(groupInfo);

    return groupId;
}


bool GroupManager::joinGroup(GroupId groupId, PeerId peerId)
{
    if (! isExists(groupId)) {
        return false;
    }

    RGroupInfo& group = getGroup(groupId);
    if (! group.join(peerId)) {
        return false;
    }

    systemService_.rpcGroupJoined(groupId, peerId);

    return true;
}


void GroupManager::groupCreated(const RGroupInfo& groupInfo)
{
    if (isExists(groupInfo.groupId_)) {
        assert(false && "same group id");
        return;
    }

    groupMap_.insert(RGroupMap::value_type(groupInfo.groupId_, groupInfo));
}


void GroupManager::groupJoined(GroupId groupId, PeerId peerId)
{
    if (! isExists(groupId)) {
        assert(false && "group not found");
        return;
    }

    RGroupInfo& group = getGroup(groupId);
    if (! group.join(peerId)) {
        assert(false && "already joined");
    }
}


GroupId GroupManager::getAvailableGroupId() const
{
    for (int i = giFirst; i < maxGroupCount; ++i) {
        const GroupId groupId = static_cast<GroupId>(i);
        if (groupMap_.find(groupId) == groupMap_.end()) {
            return groupId;
        }
    }

    return giUnknown;
}

} // namespace detail

} // namespace nsrpc
