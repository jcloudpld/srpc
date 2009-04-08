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
 * - �ν��Ͻ��� �����ϱ� ���� wait()�� ȣ���Ͽ� �����ϰ� ������ �� ���� ������
 *   ����ؾ� �Ѵ�.
 */
class NSRPC_API SessionConnector :
    public NSRPC_Asynch_Connector<NSRPC_Service_Handler>
{
    typedef NSRPC_Asynch_Connector<NSRPC_Service_Handler> Parent;
public:
    /**
     * ctor
     * @param sessionCreator ���� ���丮.
     */
    SessionConnector(SessionCreator& sessionCreator) :
        sessionCreator_(sessionCreator),
        shouldFinish_(false)
#if defined (NSRPC_USE_TPROACTOR)
        , pendingCount_(0)
#endif
        {}


    /**
     * ������ ������ �õ��Ѵ�.
     * @param serverAddr ������ IP �ּ�.
     * @param proactor Proactor ��ü. 0�� ��� singleton�� ����.
     * @param numOfConnection ���� Ƚ��.
     * @return ������ Ƚ��(<= numOfConnection).
     */
    size_t start(const ACE_INET_Addr& serverAddr, NSRPC_Proactor* proactor = 0,
        size_t numOfConnection = 1);

    /// Connector�� �����Ѵ�.
    void stop();

    /// �����ϰ� �ν��Ͻ��� ������ �� ���� ������ ����Ѵ�.
    void wait();

    /// �����ϰ� �ν��Ͻ��� ������ �� �ִ°�?
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
