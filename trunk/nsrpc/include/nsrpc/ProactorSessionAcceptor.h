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
 * - �ν��Ͻ��� �����ϱ� ���� wait()�� ȣ���Ͽ� �����ϰ� ������ �� ���� ������
 *   ����ؾ� �Ѵ�.
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
     * Acceptor�� �ʱ�ȭ�Ѵ�.
     * @param listenAddr ������� IP �ּ�.
     * @param proactor Proactor ��ü. 0�� ��� singleton�� ����.
     * @param numberOfInitialAccepts �ʱ⿡ accept�� ����.
     *        �ݵ�� 0 �̻����� �����ؾ� �Ѵ�. �ʱ� ���� ���Ӽ��� ���� ���
     *        ����� ���� ������ �����ϸ� ȿ���� ��������.
     * @return ���� ����.
     */
    bool open(const ACE_INET_Addr& listenAddr,
        NSRPC_Proactor* proactor = 0, size_t numberOfInitialAccepts = 10);

    /// ���񽺸� �����Ѵ�
    void start();

    /// Acceptor�� �ݴ´�
    void close();

    /// �����ϰ� �ν��Ͻ��� ������ �� ���� ������ ����Ѵ�
    void wait();

    /// �����ϰ� �ν��Ͻ��� ������ �� �ִ°�?
    bool isSafeToDelete() const {
        // TProactor�� ACE Proactor���� AIO cancel�� �����Ͽ� ���� ����� �ٸ�
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
