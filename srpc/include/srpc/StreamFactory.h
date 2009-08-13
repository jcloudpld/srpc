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
        stBit, ///< ��Ʈ ��Ʈ��
        stByte ///< ����Ʈ ��Ʈ��
    };

    /// �Է� ��Ʈ���� �����Ѵ�
    /// @param shouldUseUtf8ForString ���ڿ��� UTF-8�� ��ȯ?
    static std::auto_ptr<IStream> createIStream(bool shouldUseUtf8ForString,
        StreamType streamType, StreamBuffer& streamBuffer);

    /// ��� ��Ʈ���� �����Ѵ�
    /// @param shouldUseUtf8ForString ���ڿ��� UTF-8�� ��ȯ?
    static std::auto_ptr<OStream> createOStream(bool shouldUseUtf8ForString,
        StreamType streamType, StreamBuffer& streamBuffer);
};

/** @} */ // addtogroup serialization

} // namespace srpc

#endif // SRPC_STREAMFACTORY_H
