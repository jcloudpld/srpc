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
 * ACE 라이브러리를 초기화한다
 */
struct NSRPC_API InitAce
{
    InitAce();

    ~InitAce();
};


/**
 * ACE_Time_Value 객체를 만든다.
 * @param msec 밀리초(1/1000 초)
 */
inline ACE_Time_Value makeTimeValue(size_t msec)
{
    return ACE_Time_Value(msec / 1000,
        static_cast<suseconds_t>(
            (ACE_U_ONE_SECOND_IN_USECS / 1000) * (msec % 1000)));
}


/// handle이 유효한가?
inline bool isValidHandle(ACE_HANDLE handle)
{
    return handle != ACE_INVALID_HANDLE;
}


/**
 * sigmin부터 sigmax까지의 모든 시그널을 금지한다.
 * - WIN32 환경에서는 아무런 효과가 없다.
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


/// 소켓의 송신/수신 버퍼를 최대치로 설정한다.
bool NSRPC_API setMaximumSocketBufferSize(ACE_HANDLE handle);


/// Proactor 타이머를 설정한다.
long NSRPC_API setupTimer(NSRPC_Proactor& proactor,
    NSRPC_Handler& handler, size_t msec, const void* act = 0);


/// Proactor 반복 타이머를 설정한다.
long NSRPC_API setupRepeatingTimer(NSRPC_Proactor& proactor,
    NSRPC_Handler& handler, size_t msec, const void* act = 0);


/// Proactor 타이머를 해제한다
void NSRPC_API cancelTimer(NSRPC_Proactor& proactor,
    long& timerId);


/// local IP 주소를 얻는다.
InetAddresses NSRPC_API getLocalAddresses(u_short port);


/// 사설 아이피 주소인가?
bool NSRPC_API isPrivateAddress(const ACE_INET_Addr& address);


/// 공인 아이피 주소인가?
bool NSRPC_API isPublicAddress(const ACE_INET_Addr& address);


/// 공인 아이피 주소 얻기
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
