#ifndef NSRPC_ACEUTIL_H
#define NSRPC_ACEUTIL_H

#include "../config/config.h"
#include "../config/Proactor.h"
#include <ace/Message_Block.h>
#include <ace/Time_Value.h>
#include <boost/noncopyable.hpp>
#ifdef _MSC_VER
#  pragma warning (push)
#  pragma warning (disable: 4702)
#endif
#include <vector>
#ifdef _MSC_VER
#  pragma warning (pop)
#endif

class NSRPC_Handler;
class NSRPC_Proactor;

namespace nsrpc
{

/** @addtogroup utility
* @{
*/

/**
 * ACE_Time_Value ��ü�� �����.
 * @param msec �и���(1/1000 ��)
 */
inline ACE_Time_Value makeTimeValue(size_t msec)
{
    return ACE_Time_Value(msec / 1000,
        (ACE_U_ONE_SECOND_IN_USECS / 1000) * (msec % 1000));
}


/// handle�� ��ȿ�Ѱ�?
inline bool isValidHandle(ACE_HANDLE handle)
{
    return handle != ACE_INVALID_HANDLE;
}


/**
 * sigmin���� sigmax������ ��� �ñ׳��� �����Ѵ�.
 * - WIN32 ȯ�濡���� �ƹ��� ȿ���� ����.
 */
void NSRPC_API disableSignals(int sigmin = ACE_SIGRTMIN,
    int sigmax = ACE_SIGRTMAX);


/**
* @class AceMessageBlockGuard
*
* ACE_Message_Block Guard Class
*/
class AceMessageBlockGuard : public boost::noncopyable
{
public:
    AceMessageBlockGuard(ACE_Message_Block* mblk) :
        mblock_(mblk) {}

    ~AceMessageBlockGuard() {
        if (mblock_ != 0) {
            mblock_->release();
        }
    }

    ACE_Message_Block* release() {
        ACE_Message_Block* released = mblock_;
        mblock_ = 0;
        return released;
    }

    ACE_Message_Block* operator->() const {
        return mblock_;
    }

    ACE_Message_Block& operator*() const {
        assert(mblock_ != 0);
        return *mblock_;
    }

    ACE_Message_Block* get() const {
        return mblock_;
    }
private:
    ACE_Message_Block* mblock_;
};


/// ACE ���̺귯���� �ʱ�ȭ�Ѵ�
struct InitAce
{
    InitAce() {
        ACE::init();
    }

    ~InitAce() {
        ACE::fini();
    }
};


/// ������ �۽�/���� ���۸� �ִ�ġ�� �����Ѵ�.
bool NSRPC_API setMaximumSocketBufferSize(ACE_HANDLE handle);


/// Proactor Ÿ�̸Ӹ� �����Ѵ�.
long NSRPC_API setupTimer(NSRPC_Proactor& proactor,
    NSRPC_Handler& handler, size_t msec, const void* act = 0);


/// Proactor �ݺ� Ÿ�̸Ӹ� �����Ѵ�.
long NSRPC_API setupRepeatingTimer(NSRPC_Proactor& proactor,
    NSRPC_Handler& handler, size_t msec, const void* act = 0);


/// Proactor Ÿ�̸Ӹ� �����Ѵ�
void NSRPC_API cancelTimer(NSRPC_Proactor& proactor,
    long& timerId);


/// local IP �ּҸ� ��´�.
std::vector<ACE_INET_Addr> NSRPC_API getLocalAddresses(u_short port);


/// �缳 ������ �ּ��ΰ�?
bool NSRPC_API isPrivateAddress(const ACE_INET_Addr& address);


/// ���� ������ �ּ��ΰ�?
bool NSRPC_API isPublicAddress(const ACE_INET_Addr& address);


/// ���� ������ �ּ� ���
std::string NSRPC_API obtainPublicIpAddress();

/** @} */ // addtogroup utility

} // namespace nsrpc

#endif // NSRPC_ACEUTIL_H
