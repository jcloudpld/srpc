#ifndef SVOIP_VOP2PPLUGIN_H
#define SVOIP_VOP2PPLUGIN_H

#ifdef _MSC_VER
#  pragma once
#endif

#include "config/config.h"
#include <nsrpc/p2p/P2pConfig.h>
#include <nsrpc/p2p/RpcPlugIn.h>
#include <nsrpc/p2p/PeerId.h>
#include <nsrpc/p2p/Group.h>

namespace svoip
{

/**
 * @class VoP2pPlugIn
 * Voice Over P2P Plug-in
 */
class SVOIP_API VoP2pPlugIn : public nsrpc::RpcPlugIn
{
public:
    /// start recording(broadcast)
    virtual void record() = 0;

    /// start recording(unicast)
    virtual void record(nsrpc::PeerId to) = 0;

    /// start recording(multicast)
    virtual void record(nsrpc::GroupId to) = 0;

    /// stop recording
    virtual void stop() = 0;
};


typedef nsrpc::SmartPtr<VoP2pPlugIn> VoP2pPlugInPtr;

} // namespace svoip

#endif // SVOIP_VOP2PPLUGIN_H
