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
 * Reactor Event Loop�� �����ϱ� ���� Thread Pool.
 * - �ν��Ͻ��� �����ϱ� ���� stop()�� wait()�� ȣ���Ͽ� �����ϰ� ������ ��
 *   �ִ� ���±��� ����� �� �����ؾ� �Ѵ�.
 */
class NSRPC_API ReactorTask
{
public:
    /**
     * ctor.
     * @param allocReactor true�� ��� �ν��Ͻ��� �����ϰ�, false�� ���
     *                    singleton �ν��Ͻ��� ����Ѵ�.
     */
    ReactorTask(bool allocReactor = false);

    /// ��� �����带 ������ ������ ����ϰ� @c deleteReactor_�� true�� ���
    /// �޸𸮸� �����մϴ�.
    ~ReactorTask();

    /**
     * Reactor Event Loop�� ������ �����带 �����մϴ�.
     * @param reactorThreadCount reactor_event_loop�� ȣ���� ������ ����.
     */
    bool start(size_t reactorThreadCount);

    /**
     * Reactor Event Loop�� �����մϴ�.
     */
    void stop();

    /// ��� �����尡 ������ ������ ����Ѵ�
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
