#ifndef NSRPC_SESSIONCONNECTOR_H
#define NSRPC_SESSIONCONNECTOR_H

#include "nsrpc.h"

#if defined (NSRPC_HAS_PROACTOR)

#include "config/Proactor.h"
#ifdef _MSC_VER
#  pragma warning (push)
#  pragma warning (disable: 4127)
#endif
#if defined (NSRPC_USE_TPROACTOR)
#  include <TProactor/Asynch_Connector.h>
#endif // NSRPC_USE_TPROACTOR
#ifdef _MSC_VER
#  pragma warning (pop)
#endif

namespace nsrpc
{

class SessionCreator;

/** @addtogroup session
* @{
*/

/**
 * @class SessionConnector
 *
 * Asynchronous Session(connection) connector.
 * - 인스턴스를 삭제하기 전에 wait()를 호출하여 안전하게 삭제할 수 있을 때까지
 *   대기해야 한다.
 */
class NSRPC_API SessionConnector :
    public NSRPC_Asynch_Connector<NSRPC_Service_Handler>
{
    typedef NSRPC_Asynch_Connector<NSRPC_Service_Handler> Parent;
public:
    /**
     * ctor
     * @param sessionCreator 세션 팩토리.
     */
    SessionConnector(SessionCreator& sessionCreator) :
        sessionCreator_(sessionCreator),
        shouldFinish_(false)
#if defined (NSRPC_USE_TPROACTOR)
        , pendingCount_(0)
#endif
        {}


    /**
     * 서버에 접속을 시도한다.
     * @param serverAddr 연결할 IP 주소.
     * @param proactor Proactor 객체. 0일 경우 singleton이 사용됨.
     * @param numOfConnection 연결 횟수.
     * @return 성공한 횟수(<= numOfConnection).
     */
    size_t start(const ACE_INET_Addr& serverAddr, NSRPC_Proactor* proactor = 0,
        size_t numOfConnection = 1);

    /// Connector를 중지한다.
    void stop();

    /// 안전하게 인스턴스를 삭제할 수 있을 때까지 대기한다.
    void wait();

    /// 안전하게 인스턴스를 삭제할 수 있는가?
    bool isSafeToDelete() const {
#if defined (NSRPC_USE_TPROACTOR)
        return pendingCount_ <= 0;
#else
        return true;
#endif
    }
private:
    virtual NSRPC_Service_Handler* make_handler();
    virtual int validate_connection(const NSRPC_Asynch_Connect::Result& result,
        const ACE_INET_Addr& remote, const ACE_INET_Addr& local);
private:
    SessionCreator& sessionCreator_;
    bool shouldFinish_;
#if defined (NSRPC_USE_TPROACTOR)
    ACE_Atomic_Op<ACE_Thread_Mutex, long> pendingCount_;
#endif
};

/** @} */ // addtogroup session

} // namespace nsrpc

#endif // #if defined (NSRPC_HAS_PROACTOR)

#endif // NSRPC_SESSIONCONNECTOR_H
