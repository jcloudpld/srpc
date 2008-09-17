#ifndef SVOIP_RECORDERCALLBACK_H
#define SVOIP_RECORDERCALLBACK_H

#include "config/config.h"

namespace svoip
{

/**
 * @class RecorderCallback
 */
class RecorderCallback
{
public:
    virtual ~RecorderCallback() {}

    virtual void sampled(EncodedSample* sample, size_t sampleLen,
        size_t frames) = 0;
};

} // namespace svoip

#endif // SVOIP_RECORDERCALLBACK_H
