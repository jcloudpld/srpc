#ifndef SRPC_STREAMTEXTURE_H
#define SRPC_STREAMTEXTURE_H

#include "DummyStreamBuffer.h"
#include <srpc/StreamFactory.h>
#include <boost/scoped_ptr.hpp>

/**
* @class StreamTexture 
*
*/
class StreamTexture : public testing::Test
{
protected:
    virtual void SetUp() {
        ostream_.reset(
            srpc::StreamFactory::createOStream(getStreamType(), buffer_));
        istream_.reset(
            srpc::StreamFactory::createIStream(getStreamType(), buffer_));
    }

private:
    virtual srpc::StreamFactory::StreamType getStreamType() const = 0;

protected:
    DummyStreamBuffer buffer_;
    boost::scoped_ptr<srpc::OStream> ostream_;
    boost::scoped_ptr<srpc::IStream> istream_;
};


/**
* @class BitStreamTexture 
*
*/
class BitStreamTexture : public StreamTexture
{
private:
    virtual srpc::StreamFactory::StreamType getStreamType() const {
        return srpc::StreamFactory::stBit;
    }
};


/**
* @class ByteStreamTexture 
*
*/
class ByteStreamTexture : public StreamTexture
{
private:
    virtual srpc::StreamFactory::StreamType getStreamType() const {
        return srpc::StreamFactory::stByte;
    }
};

#endif // SRPC_STREAMTEXTURE_H
