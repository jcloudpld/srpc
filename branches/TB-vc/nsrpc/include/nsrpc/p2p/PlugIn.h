#ifndef NSRPC_PLUGIN_H
#define NSRPC_PLUGIN_H

#include "PeerId.h"
#include "../utility/SmartPtr.h"

namespace nsrpc
{

class P2pSession;

/** @addtogroup p2p
* @{
*/

/**
 * @class PlugIn
 * Plug-in interface
 */
class PlugIn : public SharedObject
{
public:
    virtual ~PlugIn() {}

    virtual bool initialize() = 0;

    /// attached in a P2P session instance
    virtual void attached(P2pSession* session) = 0;

    /// detached from the P2P session
    virtual void detached() = 0;

    // called from P2pSession::tick()
    virtual void update() = 0;

    /**
    * New peer connected.
    * @param peerId peer ID
    */
    virtual void onPeerConnected(PeerId peerId) = 0;

    /**
    * A peer disconnected.
    * @param peerId peer ID
    */
    virtual void onPeerDisconnected(PeerId peerId) = 0;
};


typedef SmartPtr<PlugIn> PlugInPtr;

/** @} */ // addtogroup p2p

} // namespace nsrpc

#endif // NSRPC_PLUGIN_H
