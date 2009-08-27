#ifndef NSRPC_SESSIONCONFIG_H
#define NSRPC_SESSIONCONFIG_H

#ifdef _MSC_VER
#  pragma once
#endif

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
 * @struct SessionCapacity
 * if more than maxBytesPerSecond_ bytes are sent in 1 second,
 * connection is throttled for secondsForThrottling_ seconds.
 */
struct SessionCapacity
{
    size_t maxBytesPerSecond_; ///< 0 to disable
    size_t secondsForThrottling_;

    explicit SessionCapacity(size_t maxBytesPerSecond = 0,
        size_t secondsForThrottling = 0) :
        maxBytesPerSecond_(maxBytesPerSecond),
        secondsForThrottling_(secondsForThrottling) {}

    bool isLimited() const {
        return maxBytesPerSecond_ > 0;
    }

    static SessionCapacity getNoLimitedCapacity() {
        return SessionCapacity();
    }
};


/**
* @struct SessionConfig
*/
struct SessionConfig
{
    SessionDestroyer* sessionDestroyer_;
    SynchMessageBlockManager* messageBlockManager_;
    PacketCoder* packetCoder_;
    NSRPC_Proactor* proactor_;

    SessionCapacity capacity_;

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
        PacketCoder* packetCoder, NSRPC_Proactor* proactor,
        const SessionCapacity& capacity) :
        sessionDestroyer_(sessionDestroyer),
        messageBlockManager_(messageBlockManager),
        packetCoder_(packetCoder),
        proactor_(proactor),
        capacity_(capacity) {}

    bool isValid() const {
        return (sessionDestroyer_ != 0) && (messageBlockManager_ != 0) &&
            (packetCoder_ != 0);
            // && (proactor_ != 0)
    }
};

/** @} */ // addtogroup session

} // namespace nsrpc

#endif // NSRPC_SESSIONCONFIG_H
