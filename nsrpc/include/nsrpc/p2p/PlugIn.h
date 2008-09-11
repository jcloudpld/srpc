#ifndef NSRPC_PLUGIN_H
#define NSRPC_PLUGIN_H

#include <nsrpc/utility/SmartPtr.h>

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

    /// attached in a P2P session instance
    virtual void attached(P2pSession* session) = 0;

    /// detached from the P2P session
    virtual void detached() = 0;

    virtual void update() = 0;
};


typedef SmartPtr<PlugIn> PlugInPtr;

/** @} */ // addtogroup p2p

} // namespace nsrpc

#endif // NSRPC_PLUGIN_H
