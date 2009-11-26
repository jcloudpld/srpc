#ifndef NSRPC_ACEUTIL_H
#define NSRPC_ACEUTIL_H

#ifdef _MSC_VER
#  pragma once
#endif

#include "../nsrpc.h"
#include "../config/Proactor.h"
#include <srpc/Types.h>
#include <srpc/StringTypes.h>
#include <srpc/ContainerTypes.h>
#ifdef _MSC_VER
#  pragma warning (push)
#  pragma warning (disable: 4127 4244 4267 4312 4996)
#endif
#include <ace/Thread_Mutex.h>
#include <ace/Message_Block.h>
#include <ace/Time_Value.h>
#include <ace/INET_Addr.h>
#include <ace/OS_NS_time.h>
#ifdef _MSC_VER
#  pragma warning (pop)
#endif
#include <boost/noncopyable.hpp>
#include <cassert>
#ifdef WINDOWS
#  define WIN32_LEAN_AND_MEAN // Exclude rarely-used stuff from Windows headers
#  include <windows.h>
#endif

class NSRPC_Handler;
class NSRPC_Proactor;

namespace nsrpc
{

/** @addtogroup utility
* @{
*/

/**
 * @struct InitAce
 * ACE ���̺귯���� �ʱ�ȭ�Ѵ�
 */
struct NSRPC_API InitAce
{
    InitAce();

    ~InitAce();
};


/**
 * ACE_Time_Value ��ü�� �����.
 * @param msec �и���(1/1000 ��)
 */
inline ACE_Time_Value makeTimeValue(size_t msec)
{
    return ACE_Time_Value(msec / 1000,
        static_cast<suseconds_t>(
            (ACE_U_ONE_SECOND_IN_USECS / 1000) * (msec % 1000)));
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
    explicit AceMessageBlockGuard(ACE_Message_Block* mblk = 0) :
        mblock_(mblk) {}

    ~AceMessageBlockGuard() {
        if (mblock_ != 0) {
            mblock_->release();
        }
    }

    void reset(ACE_Message_Block* mblk = 0) {
        mblock_ = mblk;
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


/// array of ACE_INET_Addr
typedef srpc::Vector<ACE_INET_Addr> InetAddresses;


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
InetAddresses NSRPC_API getLocalAddresses(u_short port);


/// �缳 ������ �ּ��ΰ�?
bool NSRPC_API isPrivateAddress(const ACE_INET_Addr& address);


/// ���� ������ �ּ��ΰ�?
bool NSRPC_API isPublicAddress(const ACE_INET_Addr& address);


/// ���� ������ �ּ� ���
srpc::String NSRPC_API obtainPublicIpAddress();


/**
* From: http://support.microsoft.com/kb/263823
* Disable "new behavior" using IOCTL: SIO_UDP_CONNRESET.
* Without this call, recvfrom() can fail, repeatedly,
* after a bad sendto() call.
* -- http://www.gamedev.net/community/forums/topic.asp?topic_id=307848
*/
void NSRPC_API workaroundWinsockConnectionResetProblem(ACE_HANDLE socket);


/**
 * @class Thread_Mutex_With_SpinLock
 * - currently, only supported in Windows
 */
class Thread_Mutex_With_SpinLock : public ACE_Thread_Mutex
{
    enum { defaultSpintCount = 4000 };
public:
    Thread_Mutex_With_SpinLock(srpc::UInt32 spinCount = defaultSpintCount,
        const ACE_TCHAR* name = 0,
        ACE_mutexattr_t* arg = 0) :
        ACE_Thread_Mutex(name, arg) {
#ifdef WINDOWS
        (void)::SetCriticalSectionSpinCount(&this->lock_, spinCount);
#else
        ACE_UNUSED_ARG(spinCount);
#endif
    }
};

/** @} */ // addtogroup utility

} // namespace nsrpc

#endif // NSRPC_ACEUTIL_H
