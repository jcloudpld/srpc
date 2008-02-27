#ifndef NSRPC_SESSIONCONFIG_H
#define NSRPC_SESSIONCONFIG_H

#include "config/Proactor.h"

namespace nsrpc
{

class SessionDestroyer;
class PacketCoder;
class SynchMessageBlockManager;

/** @addtogroup session
* @{
*/

/**
* @struct SessionConfig
*/
struct SessionConfig
{
    /**
    * ctor
    * @param sessionDestroyer ���� ������ �ν��Ͻ�.
    * @param messageBlockManager �޸� �� ������.
    * @param packetCoder PacketCoder �ν��Ͻ�.
    *          �޸� �������� �Ѱܾ� �Ѵ�(���� �Ҵ�).
    * @param proactor Proactor �ν��Ͻ�. 0�̸� �̱����� ���ȴ�.
    */
    SessionConfig(SessionDestroyer* sessionDestroyer,
        SynchMessageBlockManager* messageBlockManager,
        PacketCoder* packetCoder, NSRPC_Proactor* proactor) :
        sessionDestroyer_(sessionDestroyer),
        messageBlockManager_(messageBlockManager),
        packetCoder_(packetCoder),
        proactor_(proactor) {}

    bool isValid() const {
        return (sessionDestroyer_ != 0) && (messageBlockManager_ != 0) &&
            (packetCoder_ != 0);
            // && (proactor_ != 0)
    }

    SessionDestroyer* sessionDestroyer_;
    SynchMessageBlockManager* messageBlockManager_;
    PacketCoder* packetCoder_;
    NSRPC_Proactor* proactor_;
};

/** @} */ // addtogroup session

} // namespace nsrpc

#endif // NSRPC_SESSIONCONFIG_H
