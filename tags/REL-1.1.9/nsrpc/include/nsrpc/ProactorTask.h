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
 * Proactor Event Loop를 구동하기 위한 Thread Pool.
 * - 인스턴스를 삭제하기 전에 stop()과 wait()를 호출하여 안전하게 삭제할 수
 *   있는 상태까지 대기한 후 삭제해야 한다.
 */
class NSRPC_API ProactorTask : public ACE_Task_Base
{
public:
    /**
     * ctor.
     * @param proactor Proactor 객체. 0일 경우 singleton 객체가 사용됨.
     * @param deleteProactor true일 경우 메모리를 해제합니다.
     */
    ProactorTask(NSRPC_Proactor* proactor = 0, bool deleteProactor = false) :
        proactor_(proactor),
        deleteProactor_(deleteProactor) {}

    /// 모든 쓰레드를 종료할 때까지 대기하고 @c deleteProactor_가 true일 경우
    /// 메모리를 해제합니다.
    ~ProactorTask();

    /**
     * Proactor Event Loop를 실행할 쓰레드를 생성합니다.
     * @param proactorThreadCount proactor_event_loop를 호출할 쓰레드 갯수.
     */
    bool start(size_t proactorThreadCount);

    /**
     * Proactor Event Loop를 중지합니다.
     * - 모든 쓰레드가 종료될 때까지 대기합니다.
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
