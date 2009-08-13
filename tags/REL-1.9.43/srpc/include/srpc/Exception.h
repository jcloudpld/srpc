#ifndef SRPC_EXCEPTION_H
#define SRPC_EXCEPTION_H

#ifdef _MSC_VER
#  pragma once
#endif

#include "srpc.h"
#include "utility/CUtils.h"
#include <stdexcept>

namespace srpc {

/** @addtogroup Exception
* @{
*/

/**
 * @class Exception
 *
 * root runtime_error class.
 */
class SRPC_API_INLINE Exception : public std::runtime_error
{
    enum { MAX_BUFFER_SIZE = 512 };
public:
    Exception(const char* file, int fileno, const char* msg) :
        std::runtime_error("") {
#ifdef _MSC_VER
#  pragma warning (push)
#  pragma warning (disable: 4996)
#endif
        snprintf(what_, MAX_BUFFER_SIZE - 1, "Exception: (%s:%d), %s",
            file, fileno, msg);
#ifdef _MSC_VER
#  pragma warning (pop)
#endif
    }

    virtual const char* what() const throw() {
        return what_;
    }
private:
    char what_[MAX_BUFFER_SIZE];
};


/**
 * @class StreamingException
 *
 * ��Ʈ����(marshaling/unmarshaling) ����
 */
class SRPC_API_INLINE StreamingException : public Exception
{
public:
    StreamingException(const char* file, int fileno, const char* what) :
        Exception(file, fileno, what) {}
};


/**
 * @class UnknownRpcMethodException
 *
 * �� �� ���� RPC ��û�� ������ ���
 */
class SRPC_API_INLINE UnknownRpcMethodException : public Exception
{
public:
    UnknownRpcMethodException(const char* file, int fileno, const char* what) :
        Exception(file, fileno, what) {}
};


/**
* @class RpcFailedException
*
* RPC ��û�� ó���ϴ� ���� ������ �߻��� ���
* - ������ �����ؾ� �� ��� throw.
*/
class SRPC_API_INLINE RpcFailedException : public Exception
{
public:
    RpcFailedException(const char* file, int fileno, const char* what) :
        Exception(file, fileno, what) {}
};

/** @} */ // addtogroup Exception

} // namespace srpc

#endif // SRPC_EXCEPTION_H
