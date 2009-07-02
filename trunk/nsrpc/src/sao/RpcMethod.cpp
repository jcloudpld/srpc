#include "stdafx.h"
#include <nsrpc/sao/detail/MethodReleaser.h>
#include <nsrpc/sao/detail/RpcServant.h>
#include <nsrpc/sao/detail/RpcMethod.h>
#include <nsrpc/utility/Logger.h>
#include <ace/Message_Block.h>

namespace nsrpc
{

namespace sao
{

RpcMethod::~RpcMethod()
{
    ACE_Message_Block::release(messageBlock_);
}


void RpcMethod::setParameter(RpcServant* servant, ACE_Message_Block* mblock)
{
    assert(servant != 0);
    assert(mblock != 0);
    assert(mblock->data_block() != 0);
    assert(mblock->length() > 0);

    servant_ = servant;
    if (messageBlock_ != 0) {
        messageBlock_->reset();
        messageBlock_->size(mblock->length());
    }
    else {
        // CAUTION: don't clone!
        messageBlock_ = new ACE_Message_Block(mblock->length());
    }

    messageBlock_->copy(mblock->rd_ptr(), mblock->length());
}


void RpcMethod::call()
{
    assert(messageBlock_->data_block() != 0);
    assert(messageBlock_->length() > 0);
    //assert(servant_ != 0);

    if (servant_ != 0) {
        servant_->dispatch(*messageBlock_);
    }
    else {
        NSRPC_LOG_ERROR("RpcMethod::call() - servant absence");
    }
    release();

    servant_ = 0;
}

} // namespace sao

} // namespace nsrpc
