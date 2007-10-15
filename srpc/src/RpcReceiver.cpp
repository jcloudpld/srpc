#include "stdafx.h"
#include <srpc/RpcReceiver.h>
#include <srpc/RpcNetwork.h>
#include <srpc/IStream.h>

namespace srpc {

RpcReceiver::RpcReceiver(RpcNetwork* rpcNetwork) :
    rpcNetwork_(0),
    eventMap_(true)
{
    if (rpcNetwork != 0) {
        setRpcNetwork(rpcNetwork);
    }
}


RpcReceiver::~RpcReceiver()
{
}


bool RpcReceiver::handle(const RpcId& rpcId, IStream& istream,
    const void* rpcHint)
{
    RpcEvent* rpcEvent = lookupEvent(rpcId);
    if (! rpcEvent)
        return false;

    return handleEvent(*rpcEvent, istream, rpcHint);
}


void RpcReceiver::setRpcNetwork(RpcNetwork* rpcNetwork)
{
    assert(rpcNetwork != 0);

    rpcNetwork_ = rpcNetwork;
    if (rpcNetwork_ != 0) {
        rpcNetwork_->registerRpcReceiver(this);
    }
}


void RpcReceiver::resetRpcNetwork()
{
    if (rpcNetwork_ != 0) {
        rpcNetwork_->unregisterRpcReceiver(this);
        rpcNetwork_ = 0;
    }
}


RpcEvent* RpcReceiver::lookupEvent(const RpcId& rpcId)
{
    RpcEvent* event = eventMap_.getRpcEvent(rpcId);
    if (! event) {
        event = getDefaultEventMap().getRpcEvent(rpcId);
        if (event != 0) {
            RpcEvent* clonedEvent = event->clone();
            setRpcEvent(rpcId, clonedEvent);
        }
    }
    return event;
}


bool RpcReceiver::handleEvent(RpcEvent& rpcEvent, IStream& istream,
    const void* rpcHint)
{
    // CAUTION: ���߻������ ���� �߸��� this �����͸� �̿��Ͽ� ��� �Լ���
    // ȣ��Ǵ� ���� �����ϱ� ���� ��ȸ(indirection)��
    // - ����: http://www.codeproject.com/cpp/FastDelegate.asp
    dispatch(rpcEvent, istream, rpcHint);

    istream.align();

    return true;
}

} // namespace srpc
