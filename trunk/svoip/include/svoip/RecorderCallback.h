#ifndef SVOIP_RECORDERCALLBACK_H
#define SVOIP_RECORDERCALLBACK_H

#ifdef _MSC_VER
#  pragma once
#endif

#include "config/config.h"
#include <nsrpc/p2p/PeerId.h>
#include <nsrpc/p2p/Group.h>

namespace svoip
{

/**
 * @class RecorderCallback
 */
class RecorderCallback
{
public:
    virtual ~RecorderCallback() {}

    virtual void sampled(nsrpc::PeerId targetPeerId,
        nsrpc::GroupId targetGroupId, EncodedSample* sample, size_t sampleLen,
        size_t frames, Speech speech, Sequence sequence) = 0;
};

} // namespace svoip

#endif // SVOIP_RECORDERCALLBACK_H
