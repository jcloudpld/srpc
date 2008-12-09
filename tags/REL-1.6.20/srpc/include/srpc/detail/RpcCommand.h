#ifndef SRPC_RPCCOMMAND_H
#define SRPC_RPCCOMMAND_H

#include "../config/config.h"
#include "../Types.h"
#include "../detail/RpcId.h"
#include <boost/utility.hpp>

namespace srpc {

class OStream;
class ForwardingFunctor;

/** @addtogroup RpcForwarding
* @{
*/

/**
 * @class RpcCommand
 *
 * rpc command(request)
 * - ��û�ϴ� ���� ���ϸ� ���̱� ���� �ʿ��� ������ wrapping
 */
class SRPC_API RpcCommand : public boost::noncopyable
{
public:
    RpcCommand(const RRpcId& rpcId)  :
        rpcId_(rpcId) {}

    virtual ~RpcCommand() {}

    /**
     * RPC command�� ostream���� �������Ѵ�
     * @exception StreamingException ��Ʈ���ֿ� ������ ���
     */
    void marshal(OStream& ostream);

    const RRpcId& getRpcId() const {
        return rpcId_;
    }
private:
    virtual ForwardingFunctor& getMarshaler() = 0;
private:
    const RRpcId& rpcId_;
};

/** @} */ // addtogroup RpcForwarding

} // namespace srpc

#endif // SRPC_RPCCOMMAND_H
