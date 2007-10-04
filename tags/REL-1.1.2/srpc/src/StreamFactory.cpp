#include "stdafx.h"
#include <srpc/StreamFactory.h>
#include <srpc/detail/IBitStream.h>
#include <srpc/detail/IByteStream.h>
#include <srpc/detail/OBitStream.h>
#include <srpc/detail/OByteStream.h>

namespace srpc {

IStream* StreamFactory::createIStream(StreamType streamType,
    StreamBuffer& streamBuffer)
{
    switch (streamType) {
    case stBit:
        return new IBitStream(streamBuffer);
    case stByte:
        return new IByteStream(streamBuffer);
    }
    assert(false && "알 수 없는 스트림 타입 ");
    return 0;
}


OStream* StreamFactory::createOStream(StreamType streamType,
    StreamBuffer& streamBuffer)
{
    switch (streamType) {
    case stBit:
        return new OBitStream(streamBuffer);
    case stByte:
        return new OByteStream(streamBuffer);
    }
    assert(false && "알 수 없는 스트림 타입 ");
    return 0;
}

} // namespace srpc
