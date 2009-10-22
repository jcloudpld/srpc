#ifndef SRPC_STREAMFACTORY_H
#define SRPC_STREAMFACTORY_H

#ifdef _MSC_VER
#  pragma once
#endif

#include "srpc.h"
#include "IStream.h"
#include "OStream.h"

namespace srpc {

class StreamBuffer;

/** @addtogroup serialization
* @{
*/

/**
 * @class StreamFactory
 *
 * IStream, OStream Factory
 */
struct SRPC_API StreamFactory
{
    enum StreamType {
        stBit, ///< 비트 스트림
        stByte ///< 바이트 스트림
    };

    /// 입력 스트림을 생성한다
    /// @param shouldUseUtf8ForString 문자열을 UTF-8로 전환?
    static std::auto_ptr<IStream> createIStream(bool shouldUseUtf8ForString,
        StreamType streamType, StreamBuffer& streamBuffer);

    /// 출력 스트림을 생성한다
    /// @param shouldUseUtf8ForString 문자열을 UTF-8로 전환?
    static std::auto_ptr<OStream> createOStream(bool shouldUseUtf8ForString,
        StreamType streamType, StreamBuffer& streamBuffer);
};

/** @} */ // addtogroup serialization

} // namespace srpc

#endif // SRPC_STREAMFACTORY_H
