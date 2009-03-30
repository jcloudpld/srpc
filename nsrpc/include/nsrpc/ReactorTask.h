#ifndef NSRPC_REACTORTASK_H
#define NSRPC_REACTORTASK_H

#include "nsrpc.h"
#include <boost/scoped_ptr.hpp>

// forward decl.
class ACE_Reactor;

namespace nsrpc
{

namespace detail
{
class ReactorTaskImpl;
} // namespace detail

/** @addtogroup reactor
* @{
*/

/**
 * @class ReactorTask
 *
 * Reactor Event Loop를 구동하기 위한 Thread Pool.
 * - 인스턴스를 삭제하기 전에 stop()과 wait()를 호출하여 안전하게 삭제할 수
 *   있는 상태까지 대기한 후 삭제해야 한다.
 */
class NSRPC_API ReactorTask
{
public:
    /**
     * ctor.
     * @param allocReactor true일 경우 인스턴스를 생성하고, false일 경우
     *                    singleton 인스턴스를 사용한다.
     */
    ReactorTask(bool allocReactor = false);

    /// 모든 쓰레드를 종료할 때까지 대기하고 @c deleteReactor_가 true일 경우
    /// 메모리를 해제합니다.
    ~ReactorTask();

    /**
     * Reactor Event Loop를 실행할 쓰레드를 생성합니다.
     * @param reactorThreadCount reactor_event_loop를 호출할 쓰레드 갯수.
     */
    bool start(size_t reactorThreadCount);

    /**
     * Reactor Event Loop를 중지합니다.
     */
    void stop();

    /// 모든 쓰레드가 종료할 때까지 대기한다
    void wait();

    ACE_Reactor* getReactor() const {
        return reactor_;
    }
private:
    bool allocReactor_;
    ACE_Reactor* reactor_;

    boost::scoped_ptr<detail::ReactorTaskImpl> pimpl_;
};

/** @} */ // addtogroup reactor

} // namespace nsrpc

#endif // NSRPC_REACTORTASK_H
