#ifndef NSRPC_PLUGINMANAGER_H
#define NSRPC_PLUGINMANAGER_H

#include "nsrpc/p2p/PlugIn.h"
#include <srpc/ContainerTypes.h>

namespace nsrpc
{

namespace detail
{

/** @addtogroup p2p
* @{
*/

/**
 * @class PluginManager
 * Plug-in Manager
 */
class PluginManager : public boost::noncopyable
{
    typedef srpc::Vector<PlugInPtr> PlugIns;
public:

    void add(PlugInPtr& plugIn);
    void remove(PlugInPtr& plugIn);

    void update();

    void peerConnected(PeerId peerId);
    void peerDisconnected(PeerId peerId);

private:
    PlugIns plugIns_;
};

/** @} */ // addtogroup p2p

} // namespace detail

} // namespace nsrpc

#endif // NSRPC_PLUGINMANAGER_H
