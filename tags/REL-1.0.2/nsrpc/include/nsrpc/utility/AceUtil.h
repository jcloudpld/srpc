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
 * ACE_Time_Value 객체를 만든다.
 * @param msec 밀리초(1/1000 초)
 */
inline ACE_Time_Value makeTimeValue(size_t msec)
{
    return ACE_Time_Value(msec / 1000,
        (ACE_U_ONE_SECOND_IN_USECS / 1000) * (msec % 1000));
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


/// ACE 라이브러리를 초기화한다
struct InitAce
{
    InitAce() {
        ACE::init();
    }

    ~InitAce() {
        ACE::fini();
    }
};


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
std::vector<ACE_INET_Addr> NSRPC_API getLocalAddresses(u_short port);


/// 사설 아이피 주소인가?
bool NSRPC_API isPrivateAddress(const ACE_INET_Addr& address);


/// 공인 아이피 주소인가?
bool NSRPC_API isPublicAddress(const ACE_INET_Addr& address);


/// 공인 아이피 주소 얻기
std::string NSRPC_API obtainPublicIpAddress();

/** @} */ // addtogroup utility

} // namespace nsrpc

#endif // NSRPC_ACEUTIL_H
