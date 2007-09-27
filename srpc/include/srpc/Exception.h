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
 * 스트리밍(marshaling/unmarshaling) 에러
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
 * 알 수 없는 RPC 요청을 수신한 경우
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
* RPC 요청을 처리하는 도중 에러가 발생할 경우
* - 접속을 해제해야 할 경우 throw.
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
