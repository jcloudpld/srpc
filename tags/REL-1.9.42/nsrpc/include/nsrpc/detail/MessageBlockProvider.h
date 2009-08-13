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
 * MessageBlockManager�� �̿��ϱ� ���� mixin class.
 */
class MessageBlockProvider
{
public:
    virtual ~MessageBlockProvider() {}

    /**
     * �޼��� ���ۿ� ���� ��´�.
     * - ȹ���� ���� release()�� ���� ��ȯ�ؾ� �Ѵ�.
     */
    virtual ACE_Message_Block& acquireSendBlock() = 0;

    /**
     * �޼��� ���ſ� ���� ��´�.
     * - ȹ���� ���� release()�� ���� ��ȯ�ؾ� �Ѵ�.
     */
    virtual ACE_Message_Block& acquireRecvBlock() = 0;
};

/** @} */ // addtogroup session

} // namespace nsrpc

#endif // NSRPC_MESSAGEBLOCKPROVIDER_H
