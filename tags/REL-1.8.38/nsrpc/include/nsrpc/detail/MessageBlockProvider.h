#ifndef NSRPC_MESSAGEBLOCKPROVIDER_H
#define NSRPC_MESSAGEBLOCKPROVIDER_H

class ACE_Message_Block;

namespace nsrpc
{

/** @addtogroup session
* @{
*/

/**
 * @class MessageBlockProvider
 *
 * MessageBlockManager를 이용하기 위한 mixin class.
 */
class MessageBlockProvider
{
public:
    virtual ~MessageBlockProvider() {}

    /**
     * 메세지 전송용 블럭을 얻는다.
     * - 획득한 블럭은 release()를 통해 반환해야 한다.
     */
    virtual ACE_Message_Block& acquireSendBlock() = 0;

    /**
     * 메세지 수신용 블럭을 얻는다.
     * - 획득한 블럭은 release()를 통해 반환해야 한다.
     */
    virtual ACE_Message_Block& acquireRecvBlock() = 0;
};

/** @} */ // addtogroup session

} // namespace nsrpc

#endif // NSRPC_MESSAGEBLOCKPROVIDER_H
