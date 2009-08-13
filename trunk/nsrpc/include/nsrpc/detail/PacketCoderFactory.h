#ifndef NSRPC_PACKETCODERFACTORY_H
#define NSRPC_PACKETCODERFACTORY_H

#ifdef _MSC_VER
#  pragma once
#endif

#include "../nsrpc.h"
#include <boost/noncopyable.hpp>

namespace nsrpc
{

class PacketCoder;

/** @addtogroup protocol
* @{
*/

/**
 * @class PacketCoderFactory
 * PacketCodeer factory
 * - Abstract Factory Pattern
 */
class NSRPC_API PacketCoderFactory : public boost::noncopyable
{
public:
    virtual ~PacketCoderFactory() {}

    /**
     * C/S�� PacketCoder�� �����Ѵ�.
     */
    virtual PacketCoder* create() const;

    /**
     * P2P�� PacketCoder�� �����Ѵ�.
     */
    virtual PacketCoder* createForP2p() const;
};

/** @} */ // addtogroup protocol

} // namespace nsrpc

#endif // !defined(NSRPC_PACKETCODERFACTORY_H)
