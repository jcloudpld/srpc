#ifndef NSRPC_PROACTORSESSION_H
#define NSRPC_PROACTORSESSION_H

#ifdef _MSC_VER
#  pragma once
#endif

#include "nsrpc.h"
#include "config/Proactor.h"

#if defined (NSRPC_HAS_PROACTOR)

#include "detail/MessageBlockProvider.h"
#include "detail/CsProtocol.h"
#include "Session.h"
#include <srpc/StringTypes.h>
#ifdef _MSC_VER
#  pragma warning (push)
#  pragma warning (disable: 4127 4244 4251 4267 4312 4996)
#endif
#if defined (NSRPC_USE_TPROACTOR)
#  include <TProactor/Asynch_IO.h>
#else
#  include <ace/Asynch_IO.h>
#endif // NSRPC_USE_TPROACTOR
#include <ace/INET_Addr.h>
#include <ace/Atomic_Op.h>
#include <ace/Thread_Mutex.h>
#ifdef _MSC_VER
#  pragma warning (pop)
#endif
#include <boost/scoped_ptr.hpp>

namespace nsrpc
{

class Asynch_RW_Stream;
class SessionDestroyer;
class SynchMessageBlockManager;
class PacketCoder;
class BandwidthLimit;
struct SessionConfig;

/** @addtogroup session
* @{
*/

/**
 * @class ProactorSession
 *
 * Asynchronous ProactorSession
 * - CAUTION: 반드시 recvLock_을 건 후 sendLock_을 걸어야 한다
 */
class NSRPC_API ProactorSession : public Session,
    public NSRPC_Service_Handler, protected MessageBlockProvider
{
public:
    /**
     * @struct Stats
     * 세션의 통계 정보를 기록한다.
     */
    struct Stats
    {
        size_t recvBytes_;
        size_t sentBytes_;
        size_t useCount_;

        Stats() :
            useCount_(0) {
            reset();
        }

        void reset() {
            recvBytes_ = 0;
            sentBytes_ = 0;
            ////useCount_ = 0;
        }
    };
public:
    ProactorSession(const SessionConfig& config);
    virtual ~ProactorSession();

    virtual void closeConnection() {
        disconnect();
    }
public:
    /// remote host에 접속한다(ACE_Socket_Connector 이용)
    bool connect(const srpc::String& host, u_short port, size_t timeout);

    /**
     * 메세지를 전송한다. 전송이 실패할 경우 접속을 해제한다.
     * @param mblock 전송할 버퍼. 반드시 acquireSendBlock() 호출로 인스턴스를
     *               획득해야 한다.
     * @param messageType 메세지 타입
     */
    void sendMessage(ACE_Message_Block& mblock,
        CsMessageType messageType = mtGeneral);

    /// AIO를 취소한다. AIO 에러를 일으켜 세션을 종료하게 한다.
    void disconnect();

    /**
     * 전송을 완료한 후 접속을 해제한다
     * - 클라이언트가 FIN을 받은 후 접속을 해제하지 않으면, 서버가 접속해제를
     *   할 수 없으므로 별도의 접속해제 타이머를 둬서 강제로 접속을 해제한다.
     */
    void disconnectGracefully();

    /// 접속 중인가?
    bool isConnected() const;

    /// 통계 정보를 얻는다.
    const Stats& getStats() const {
        return stats_;
    }

    /// 클라이언트 세션이면 클라이언트의 IP 주소
    const ACE_INET_Addr& getRemoteAddress() const {
        return remoteAddress_;
    }

    /// 클라이언트 세션이면 서버 측에 바인딩된 IP 주소
    const ACE_INET_Addr& getPrivateAddress() const {
        return localAddress_;
    }
protected:
    PacketCoder& getPacketCoder() {
        return *packetCoder_;
    }
    ACE_Thread_Mutex& getSendLock() {
        return sendLock_;
    }
    ACE_Thread_Mutex& getRecvLock() {
        return recvLock_;
    }
protected:
    /// 접속될 경우 호출된다.
    /// @return false이면 접속을 해제한다
    virtual bool onConnected() {
        return true;
    }
private:
    /// 클라이언트로 부터 메세지가 도착하였다.
    virtual bool onMessageArrived(CsMessageType messageType) = 0;
    virtual void onThrottling(size_t readBytes, size_t maxBytesPerSecond);
    /// 접속이 해제될 경우 호출된다.
    virtual void onDisconnected() = 0;
public: // for Test
    // = MessageBlockProvider overriding
    virtual ACE_Message_Block& acquireSendBlock();
    virtual ACE_Message_Block& acquireRecvBlock();
protected:
    // = NSRPC_Service_Handler overriding
    virtual void open(ACE_HANDLE new_handle, ACE_Message_Block& message_block);
    virtual void handle_read_stream(
        const NSRPC_Asynch_Read_Stream::Result& result);
    virtual void handle_write_stream(
        const NSRPC_Asynch_Write_Stream::Result& result);
    virtual void handle_time_out(const ACE_Time_Value& tv, const void* act);
    virtual void addresses(const ACE_INET_Addr& remote_address,
        const ACE_INET_Addr& local_address);
private:
    bool readMessage(const NSRPC_Asynch_Read_Stream::Result& result);
    bool readMessageHeader();
    bool readMessageFragment(size_t neededBytes);
    bool read(size_t neededBytes);

    bool write(ACE_Message_Block& mblock);

    void reset();

    void startDisconnectTimer();
    void stopDisconnectTimer();

    void startThrottleTimer();
    void stopThrottleTimer();

    void checkPendingCount();

    bool isSafeToDelete() const {
        return (pendingReadCount_ <= 0) && (pendingWriteCount_ <= 0);
    }
private:
    SessionDestroyer& sessionDestroyer_;
    SynchMessageBlockManager& messageBlockManager_;

    NSRPC_Proactor* proactor_;
    boost::scoped_ptr<PacketCoder> packetCoder_;
    CsPacketHeader headerForReceive_;

    ACE_Message_Block* recvBlock_;

    boost::scoped_ptr<Asynch_RW_Stream> stream_;

    ACE_INET_Addr remoteAddress_;
    ACE_INET_Addr localAddress_;

    ACE_Atomic_Op<ACE_Thread_Mutex, long> pendingReadCount_;
    ACE_Atomic_Op<ACE_Thread_Mutex, long> pendingWriteCount_;
    long prevLoggedPendingWriteCount_;

    long disconnectTimer_;
    bool disconnectReserved_;

    boost::scoped_ptr<BandwidthLimit> inboundBandwidthLimiter_;
    long throttleTimer_;

    Stats stats_;

    ACE_Thread_Mutex sendLock_;
    ACE_Thread_Mutex recvLock_;
};

/** @} */ // addtogroup session

} // namespace nsrpc

#endif // #if defined (NSRPC_HAS_PROACTOR)

#endif // NSRPC_PROACTORSESSION_H
