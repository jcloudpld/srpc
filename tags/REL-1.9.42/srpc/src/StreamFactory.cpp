#include "stdafx.h"
#include <srpc/StreamFactory.h>
#include <srpc/detail/IBitStream.h>
#include <srpc/detail/IByteStream.h>
#include <srpc/detail/OBitStream.h>
#include <srpc/detail/OByteStream.h>

namespace srpc {

std::auto_ptr<IStream> StreamFactory::createIStream(
    bool shouldUseUtf8ForString,
    StreamType streamType, StreamBuffer& streamBuffer)
{
    switch (streamType) {
    case stBit:
        return std::auto_ptr<IStream>(
            new IBitStream(streamBuffer, shouldUseUtf8ForString));
    case stByte:
        return std::auto_ptr<IStream>(
            new IByteStream(streamBuffer, shouldUseUtf8ForString));
    }
    assert(false && "Unknown stream type");
    return std::auto_ptr<IStream>();
}


std::auto_ptr<OStream> StreamFactory::createOStream(bool shouldUseUtf8ForString,
    StreamType streamType, StreamBuffer& streamBuffer)
{
    switch (streamType) {
    case stBit:
        return std::auto_ptr<OStream>(
            new OBitStream(streamBuffer, shouldUseUtf8ForString));
    case stByte:
        return std::auto_ptr<OStream>(
            new OByteStream(streamBuffer, shouldUseUtf8ForString));
    }
    assert(false && "Unknown stream type");
    return std::auto_ptr<OStream>();
}

} // namespace srpc
