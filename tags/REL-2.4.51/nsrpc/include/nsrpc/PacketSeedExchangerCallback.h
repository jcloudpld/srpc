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
 * 패킷 시드 교환 콜백
 */
class NSRPC_API PacketSeedExchangerCallback : public boost::noncopyable
{
public:
    virtual ~PacketSeedExchangerCallback() {}

    /// 공개키 및 개인키를 생성한다
    /// - 클라이언트/서버 공용
    /// @return 키값이 유효한가?
    virtual bool generatePublicAndPrivateKey(
        srpc::String& publicKey, srpc::String& privateKey) {
        SRPC_UNUSED_ARG(publicKey);
        SRPC_UNUSED_ARG(privateKey);
        return false;
    }

    /// 상대방의 공개키를 이용하여 시드값을 암호화한다
    /// - 서버 전용
    virtual void encryptSeed(srpc::String& seed,
        const srpc::String& otherSidePublicKey) {
        SRPC_UNUSED_ARG(seed);
        SRPC_UNUSED_ARG(otherSidePublicKey);
    }

    /// 암호화된 시드값을 복호화한다
    /// - 클라이언트 전용
    /// @return 성공 여부
    virtual bool decryptSeed(srpc::String& seed) {
        SRPC_UNUSED_ARG(seed);
        return true;
    }

    /// 시드 값이 교환되었다.
    virtual void seedExchanged() {}
};

} // namespace nsrpc

#endif // !defined(NSRPC_PACKETSEEDEXCHANGERCALLBACK_H)
