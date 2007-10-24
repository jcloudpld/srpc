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
        stBit, ///< ��Ʈ ��Ʈ��
        stByte ///< ����Ʈ ��Ʈ��
    };

    /// �Է� ��Ʈ���� �����Ѵ�
    static IStream* createIStream(StreamType streamType,
        StreamBuffer& streamBuffer);

    /// ��� ��Ʈ���� �����Ѵ�
    static OStream* createOStream(StreamType streamType,
        StreamBuffer& streamBuffer);
};

/** @} */ // addtogroup serialization

} // namespace srpc

#endif // SRPC_STREAMFACTORY_H
