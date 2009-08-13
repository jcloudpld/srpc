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
        setRpcNetwork(*rpcNetwork);
    }
}


RpcReceiver::~RpcReceiver()
{
}


bool RpcReceiver::handle(RpcId rpcId, IStream& istream,
    const void* rpcHint)
{
    if (shouldCloneRpcEvents()) {
        cloneRpcEvents();
    }

    RpcEvent* rpcEvent = lookupEvent(rpcId);
    if (! rpcEvent)
        return false;

    return handleEvent(*rpcEvent, istream, rpcHint);
}


void RpcReceiver::setRpcNetwork(RpcNetwork& rpcNetwork)
{
    rpcNetwork_ = &rpcNetwork;
    if (rpcNetwork_ != 0) {
        rpcNetwork_->registerRpcReceiver(*this);
    }
}


void RpcReceiver::resetRpcNetwork()
{
    if (rpcNetwork_ != 0) {
        rpcNetwork_->unregisterRpcReceiver(*this);
        rpcNetwork_ = 0;
    }
}


bool RpcReceiver::handleEvent(RpcEvent& rpcEvent, IStream& istream,
    const void* rpcHint)
{
    onReceiving(rpcEvent.getRpcId());

    // CAUTION: ���߻������ ���� �߸��� this �����͸� �̿��Ͽ� ��� �Լ���
    // ȣ��Ǵ� ���� �����ϱ� ���� ��ȸ(indirection)��
    // - ����: http://www.codeproject.com/cpp/FastDelegate.asp
    dispatch(rpcEvent, istream, rpcHint);

    istream.align();

    return true;
}


void RpcReceiver::cloneRpcEvents()
{
    const RpcEventMap::RpcEvents& defaultEvents =
        getDefaultEventMap().getRpcEvents();
    RpcEventMap::RpcEvents::const_iterator pos = defaultEvents.begin();
    const RpcEventMap::RpcEvents::const_iterator end = defaultEvents.end();
    for (; pos != end; ++pos) {
        const RpcId rpcId = (*pos).first;
        const RpcEvent* event = (*pos).second;
        insertRpcEvent(rpcId, *(event->clone().release()));
    }
}

} // namespace srpc
