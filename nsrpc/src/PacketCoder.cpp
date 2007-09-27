#include "stdafx.h"
#include <nsrpc/detail/PacketCoder.h>
#include <nsrpc/utility/Logger.h>
#include <srpc/utility/Hash.h>
#include <sstream>

namespace nsrpc
{

PacketCoder::Seed PacketCoder::generateCipherSeed(srpc::UInt32 base) const
{
    std::ostringstream oss;
    oss << base;
    const std::string baseString(oss.str());
    oss.str("");
    oss << srpc::hash(baseString.c_str(), baseString.length());
    return oss.str();
}


void PacketCoder::reserveHeader(ACE_Message_Block& mblock)
{
    mblock.reset();

    mblock.wr_ptr(getHeaderSize());
}


bool PacketCoder::readHeader(AbstractPacketHeader& header,
    const ACE_Message_Block& mblock) const
{
    if (! isHeadOfPacket(mblock)) {
        return false;
    }

    if (! isValidPacketHeader(mblock)) {
        return false;
    }

    (void)header.read(mblock.base());

    return header.isValid();
}


bool PacketCoder::encode(ACE_Message_Block& mblock,
    AbstractPacketHeader& header)
{
    if (! isHeadOfPacket(mblock)) {
        return false;
    }

    if (! isValidPacket(mblock)) {
        return false;
    }

    if (! header.isValid()) {
        header.bodySize_ = getBodySize(mblock);
    }

    (void)header.write(mblock.base());

    return header.isValid();
}


bool PacketCoder::decode(ACE_Message_Block& mblock)
{
    if (! isHeadOfPacket(mblock)) {
        return false;
    }

    return true;
}

} // namespace nsrpc
