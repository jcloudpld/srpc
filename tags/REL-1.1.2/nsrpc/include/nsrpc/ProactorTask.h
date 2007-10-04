#ifndef NSRPC_PROACTORTASK_H
#define NSRPC_PROACTORTASK_H

#include "config/config.h"
#include "config/Proactor.h"
#ifdef _MSC_VER
#  pragma warning (push)
#  pragma warning (disable: 4127 4355 4800)
#endif
#include <ace/Task.h>
#ifdef _MSC_VER
#  pragma warning (pop)
#endif

// forward decl.
class NSRPC_Proactor;

namespace nsrpc
{

/** @addtogroup proactor
* @{
*/

/**
 * @class ProactorTask
 *
 * Proactor Event Loop�� �����ϱ� ���� Thread Pool.
 * - �ν��Ͻ��� �����ϱ� ���� stop()�� wait()�� ȣ���Ͽ� �����ϰ� ������ ��
 *   �ִ� ���±��� ����� �� �����ؾ� �Ѵ�.
 */
class NSRPC_API ProactorTask : public ACE_Task_Base
{
public:
    /**
     * ctor.
     * @param proactor Proactor ��ü. 0�� ��� singleton ��ü�� ����.
     * @param deleteProactor true�� ��� �޸𸮸� �����մϴ�.
     */
    ProactorTask(NSRPC_Proactor* proactor = 0, bool deleteProactor = false) :
        proactor_(proactor),
        deleteProactor_(deleteProactor) {}

    /// ��� �����带 ������ ������ ����ϰ� @c deleteProactor_�� true�� ���
    /// �޸𸮸� �����մϴ�.
    ~ProactorTask();

    /**
     * Proactor Event Loop�� ������ �����带 �����մϴ�.
     * @param proactorThreadCount proactor_event_loop�� ȣ���� ������ ����.
     */
    bool start(size_t proactorThreadCount);

    /**
     * Proactor Event Loop�� �����մϴ�.
     * - ��� �����尡 ����� ������ ����մϴ�.
     */
    void stop();

    NSRPC_Proactor* getProactor() const {
        return proactor_;
    }
private:
    virtual int svc();
private:
    NSRPC_Proactor* proactor_;
    bool deleteProactor_;
};

/** @} */ // addtogroup proactor

} // namespace nsrpc

#endif // NSRPC_PROACTORTASK_H
