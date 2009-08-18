#ifndef NSRPC_PACKETSEEDEXCHANGERCALLBACK_H
#define NSRPC_PACKETSEEDEXCHANGERCALLBACK_H

#ifdef _MSC_VER
#  pragma once
#endif

#include "nsrpc.h"
#include <srpc/Types.h>
#include <srpc/srpc_macros.h>
#include <boost/noncopyable.hpp>
#include <string>

namespace nsrpc
{

/**
 * @class PacketSeedExchangerCallback
 * ��Ŷ �õ� ��ȯ �ݹ�
 */
class NSRPC_API PacketSeedExchangerCallback : public boost::noncopyable
{
public:
    virtual ~PacketSeedExchangerCallback() {}

    /// ����Ű �� ����Ű�� �����Ѵ�
    /// - Ŭ���̾�Ʈ/���� ����
    /// @return Ű���� ��ȿ�Ѱ�?
    virtual bool generatePublicAndPrivateKey(
        srpc::String& publicKey, srpc::String& privateKey) {
        SRPC_UNUSED_ARG(publicKey);
        SRPC_UNUSED_ARG(privateKey);
        return false;
    }

    /// ������ ����Ű�� �̿��Ͽ� �õ尪�� ��ȣȭ�Ѵ�
    /// - ���� ����
    virtual void encryptSeed(srpc::String& seed,
        const srpc::String& otherSidePublicKey) {
        SRPC_UNUSED_ARG(seed);
        SRPC_UNUSED_ARG(otherSidePublicKey);
    }

    /// ��ȣȭ�� �õ尪�� ��ȣȭ�Ѵ�
    /// - Ŭ���̾�Ʈ ����
    /// @return ���� ����
    virtual bool decryptSeed(srpc::String& seed) {
        SRPC_UNUSED_ARG(seed);
        return true;
    }

    /// �õ� ���� ��ȯ�Ǿ���.
    virtual void seedExchanged() {}
};

} // namespace nsrpc

#endif // !defined(NSRPC_PACKETSEEDEXCHANGERCALLBACK_H)
