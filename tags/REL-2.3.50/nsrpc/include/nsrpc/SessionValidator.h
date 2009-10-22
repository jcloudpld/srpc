#ifndef NSRPC_SESSIONVALIDATOR_H
#define NSRPC_SESSIONVALIDATOR_H

#ifdef _MSC_VER
#  pragma once
#endif

#include "nsrpc.h"

namespace nsrpc
{

/** @addtogroup session
* @{
*/

/**
 * @class SessionValidator
 *
 */
class NSRPC_API SessionValidator : public boost::noncopyable
{
public:
    virtual ~SessionValidator() {}

    /// ������ ������ �� �ִ°�?
    virtual bool isValidated() const = 0;
};

/** @} */ // addtogroup session

} // namespace nsrpc

#endif // NSRPC_SESSIONVALIDATOR_H
