#ifndef NSRPC_SESSIONRPCHINT_H
#define NSRPC_SESSIONRPCHINT_H

#ifdef _MSC_VER
#  pragma once
#endif

#include "CsProtocol.h"

namespace nsrpc
{

/** @addtogroup session
* @{
*/

/**
 * @struct SessionRpcHint
 */
struct SessionRpcHint
{
    void* sendBlock_;
    CsMessageType messageType_;

    explicit SessionRpcHint(void* sendBlock = 0,
        CsMessageType messageType = mtGeneral) :
        sendBlock_(sendBlock),
        messageType_(messageType) {}

    bool isValidSendBlock() const {
        return sendBlock_ != 0;
    }
};


inline SessionRpcHint toSessionRpcHint(const void* rpcHint)
{
    if (! rpcHint) {
        return SessionRpcHint();
    }

    return *static_cast<const SessionRpcHint*>(rpcHint);
}

/** @} */ // addtogroup session

} // namespace nsrpc

#endif // !defined(NSRPC_SESSIONRPCHINT_H)
