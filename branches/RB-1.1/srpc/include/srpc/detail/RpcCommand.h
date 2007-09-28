#ifndef SRPC_RPCCOMMAND_H
#define SRPC_RPCCOMMAND_H

#include <srpc/Types.h>
#include <srpc/config/config.h>
#include <srpc/detail/RpcId.h>
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
    RpcCommand(const RpcId& rpcId, ForwardingFunctor& functor)  :
        rpcId_(rpcId),
        marshaler_(functor) {}

    /**
     * RPC command�� ostream���� �������Ѵ�
     * @exception StreamingException ��Ʈ���ֿ� ������ ���
     */
    void marshal(OStream& ostream);

    const RpcId& getRpcId() const {
        return rpcId_;
    }
private:
    RpcId rpcId_;
    ForwardingFunctor& marshaler_;
};

/** @} */ // addtogroup RpcForwarding

} // namespace srpc

#endif // SRPC_RPCCOMMAND_H
