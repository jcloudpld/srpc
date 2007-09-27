#ifndef SRPC_EXCEPTION_H
#define SRPC_EXCEPTION_H

#include "config/config.h"
#include <sstream>
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
public:
    Exception(const char* file, int fileno, const char* msg) :
        std::runtime_error("") {
        std::ostringstream oss;
        oss << "Exception: (" << file << "," << fileno << "), " << msg;
        what_ = oss.str();
    }

    virtual const char* what() const {
        return what_.c_str();
    }
private:
    std::string what_;
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
