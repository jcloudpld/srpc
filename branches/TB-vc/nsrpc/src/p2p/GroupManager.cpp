#include "stdafx.h"
#include "GroupManager.h"

namespace nsrpc
{

namespace detail
{

GroupManager::GroupManager()
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

    return groupId;
}


void GroupManager::addGroup(const RGroupInfo& groupInfo)
{
    if (isExists(groupInfo.groupId_)) {
        assert(false && "same group id");
        return;
    }

    groupMap_.insert(RGroupMap::value_type(groupInfo.groupId_, groupInfo));
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
