#ifndef SRPC_STREAMFACTORY_H
#define SRPC_STREAMFACTORY_H

#include "config/config.h"
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
    static IStream* createIStream(StreamType streamType,
        StreamBuffer& streamBuffer);

    /// 출력 스트림을 생성한다
    static OStream* createOStream(StreamType streamType,
        StreamBuffer& streamBuffer);
};

/** @} */ // addtogroup serialization

} // namespace srpc

#endif // SRPC_STREAMFACTORY_H
