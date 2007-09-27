#ifndef NSRPC_PACKETCODERFACTORY_H
#define NSRPC_PACKETCODERFACTORY_H

#include "../config/config.h"
#include <string>

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
