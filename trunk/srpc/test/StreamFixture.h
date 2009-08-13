#ifndef SRPC_STREAMTEXTURE_H
#define SRPC_STREAMTEXTURE_H

#ifdef _MSC_VER
#  pragma once
#endif

#include "DummyStreamBuffer.h"
#include <srpc/StreamFactory.h>
#include <boost/scoped_ptr.hpp>

/**
* @class StreamFixture 
*
*/
class StreamFixture : public testing::Test
{
protected:
    static const bool shouldUseUtf8ForString = true;
    static const size_t bytesForRpcChar =
        shouldUseUtf8ForString ? 1 : sizeof(wchar_t);

protected:
    virtual void SetUp() {
        ostream_.reset(srpc::StreamFactory::createOStream(
            shouldUseUtf8ForString, getStreamType(), buffer_).release());
        istream_.reset(srpc::StreamFactory::createIStream(
            shouldUseUtf8ForString, getStreamType(), buffer_).release());
    }

private:
    virtual srpc::StreamFactory::StreamType getStreamType() const = 0;

protected:
    DummyStreamBuffer buffer_;
    boost::scoped_ptr<srpc::OStream> ostream_;
    boost::scoped_ptr<srpc::IStream> istream_;
};


/**
* @class BitStreamFixture 
*
*/
class BitStreamFixture : public StreamFixture
{
private:
    virtual srpc::StreamFactory::StreamType getStreamType() const {
        return srpc::StreamFactory::stBit;
    }
};


/**
* @class ByteStreamFixture 
*
*/
class ByteStreamFixture : public StreamFixture
{
private:
    virtual srpc::StreamFactory::StreamType getStreamType() const {
        return srpc::StreamFactory::stByte;
    }
};

#endif // SRPC_STREAMTEXTURE_H
