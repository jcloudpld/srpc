#ifndef NSRPC_PROACTORSESSIONACCEPTOR_H
#define NSRPC_PROACTORSESSIONACCEPTOR_H

#include "nsrpc.h"

#if defined (NSRPC_HAS_PROACTOR)

#include "config/Proactor.h"
#ifdef _MSC_VER
#  pragma warning (push)
#  pragma warning (disable: 4127 4267 4312)
#endif
#if defined (NSRPC_USE_TPROACTOR)
#  include <TProactor/Asynch_Acceptor.h>
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
 * @class ProactorSessionAcceptor
 *
 * Asynchronous Session(connection) Acceptor.
 * - 인스턴스를 삭제하기 전에 wait()를 호출하여 안전하게 삭제할 수 있을 때까지
 *   대기해야 한다.
 */
class NSRPC_API ProactorSessionAcceptor :
    public NSRPC_Asynch_Acceptor<NSRPC_Service_Handler>
{
    typedef NSRPC_Asynch_Acceptor<NSRPC_Service_Handler> Parent;
public:
    ProactorSessionAcceptor(SessionCreator& sessionCreator) :
        sessionCreator_(sessionCreator),
        started_(false),
        shouldFinish_(false)
#if defined (NSRPC_USE_TPROACTOR)
        , pendingCount_(0)
#endif
        {}

    /**
     * Acceptor를 초기화한다.
     * @param listenAddr 연결받을 IP 주소.
     * @param proactor Proactor 객체. 0일 경우 singleton이 사용됨.
     * @param numberOfInitialAccepts 초기에 accept할 갯수.
     *        반드시 0 이상으로 설정해야 한다. 초기 동시 접속수가 많을 경우
     *        충분히 높은 값으로 설정하면 효율이 좋아진다.
     * @return 성공 여부.
     */
    bool open(const ACE_INET_Addr& listenAddr,
        NSRPC_Proactor* proactor = 0, size_t numberOfInitialAccepts = 10);

    /// 서비스를 시작한다
    void start();

    /// Acceptor를 닫는다
    void close();

    /// 안전하게 인스턴스를 삭제할 수 있을 때까지 대기한다
    void wait();

    /// 안전하게 인스턴스를 삭제할 수 있는가?
    bool isSafeToDelete() const {
        // TProactor와 ACE Proactor간에 AIO cancel과 관련하여 동작 방식이 다름
#if defined (NSRPC_USE_TPROACTOR)
        return pendingCount_ <= 0;
#else
        return true;
#endif;
    }
private:
    virtual NSRPC_Service_Handler* make_handler();
    virtual int validate_connection(const NSRPC_Asynch_Accept::Result& result,
        const ACE_INET_Addr& remote, const ACE_INET_Addr& local);
    virtual int should_reissue_accept();
private:
    SessionCreator& sessionCreator_;
    bool started_;
    bool shouldFinish_;
#if defined (NSRPC_USE_TPROACTOR)
    ACE_Atomic_Op<ACE_Thread_Mutex, long> pendingCount_;
#endif
};

/** @} */ // addtogroup session

} // namespace nsrpc

#endif // #if defined (NSRPC_HAS_PROACTOR)

#endif // NSRPC_PROACTORSESSIONACCEPTOR_H
