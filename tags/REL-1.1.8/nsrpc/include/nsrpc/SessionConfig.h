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
    * @param sessionDestroyer 세션 관리자 인스턴스.
    * @param messageBlockManager 메모리 블럭 관리자.
    * @param packetCoder PacketCoder 인스턴스.
    *          메모리 소유권을 넘겨야 한다(동적 할당).
    * @param proactor Proactor 인스턴스. 0이면 싱글톤이 사용된다.
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
