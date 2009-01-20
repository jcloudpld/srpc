#include "stdafx.h"
#include "PluginManager.h"

namespace nsrpc
{

namespace detail
{

void PluginManager::add(PlugInPtr& plugIn)
{
    plugIns_.push_back(plugIn);
}


void PluginManager::remove(PlugInPtr& plugIn)
{
    const PlugIns::iterator pos =
        std::find(plugIns_.begin(), plugIns_.end(), plugIn);
    if (pos == plugIns_.end()) {
        return;
    }

    plugIns_.erase(pos);
}


void PluginManager::update()
{
    PlugIns::iterator pos = plugIns_.begin();
    const PlugIns::iterator end = plugIns_.end();
    for (; pos != end; ++pos) {
        PlugInPtr& plugIn = *pos;
        plugIn->update();
    }
}


void PluginManager::peerConnected(PeerId peerId)
{
    PlugIns::iterator pos = plugIns_.begin();
    const PlugIns::iterator end = plugIns_.end();
    for (; pos != end; ++pos) {
        PlugInPtr& plugIn = *pos;
        plugIn->onPeerConnected(peerId);
    }
}


void PluginManager::peerDisconnected(PeerId peerId)
{
    PlugIns::iterator pos = plugIns_.begin();
    const PlugIns::iterator end = plugIns_.end();
    for (; pos != end; ++pos) {
        PlugInPtr& plugIn = *pos;
        plugIn->onPeerDisconnected(peerId);
    }
}

} // namespace detail

} // namespace nsrpc
