#if !defined(NSRPC_REACTORSESSION_H)
#define NSRPC_REACTORSESSION_H

#include "nsrpc.h"
#include "Session.h"
#include "detail/CsProtocol.h"
#include "detail/MessageBlockProvider.h"
#include <srpc/StringTypes.h>
#ifdef _MSC_VER
#  pragma warning( push)
#  pragma warning( disable : 4127 4244 4251 4267 4311 4312 4355 4996)
#endif
#include <ace/Event_Handler.h>
#include <ace/Reactor_Notification_Strategy.h>
#include <ace/SOCK_Stream.h>
#include <ace/Message_Block.h>
#include <ace/Recursive_Thread_Mutex.h>
#include <ace/Null_Condition.h>
#include <ace/Message_Queue_T.h>
#ifdef _MSC_VER
#  pragma warning( pop)
#endif
#include <boost/scoped_ptr.hpp>
#include <boost/noncopyable.hpp>

class ACE_Reactor;

namespace nsrpc
{

class PacketCoder;
class PacketCoderFactory;
class SynchMessageBlockManager;

/**
 * @class ReactorSession
 * ACE_Reactor 기반의 Session
 * - disable Nagle-algorithm
 * - set non-blocking
 * - set maximum socket buffer size
 */
class NSRPC_API ReactorSession : public Session,
    public ACE_Event_Handler, protected MessageBlockProvider
{
public:
    /**
     * @reactor Reactor instance.
     * @param packetCoderFactory PacketCoder Factory.
     *          0이면 디폴트 PacketCoder가 사용된다.
     */
    ReactorSession(ACE_Reactor* reactor = 0,
        PacketCoderFactory* packetCoderFactory = 0);
    virtual ~ReactorSession();

    virtual void cancelConnection() {
        disconnect();
    }
public:
    /**
     * 서버에 접속합니다.
     * @param ip 서버의 IP 주소
     * @param port 서버의 포트 번호
     * @param timeout 접속 시간 제한 (밀리초 단위)
     */
    bool connect(const srpc::String& ip, u_short port, size_t timeout);

    /**
     * 서버로 부터 접속을 해제합니다.
     * @param fireEvent disconnected() 이벤트를 발생시킬 것인가?
     */
    void disconnect(bool fireEvent = false);

    /**
     * 전송 큐에 있는 모든 데이터를 전송한 후 접속을 해제한다.
     */

    void disconnectGracefully(bool fireEvent = false);

    /**
     * 메세지를 전송한다. 전송이 실패할 경우 접속을 해제한다.
     * @param mblock 전송할 버퍼. 반드시 acquireSendBlock() 호출로 인스턴스를
     *               획득해야 한다.
     */
    bool sendMessage(ACE_Message_Block& mblock, CsMessageType messageType);

    /// 접속 중인가?
    bool isConnected() const {
        return get_handle() != ACE_INVALID_HANDLE;
    }
protected:
    PacketCoder& getPacketCoder();
    ACE_Recursive_Thread_Mutex& getLock() {
        return lock_;
    }
    ACE_SOCK_Stream& peer() {
        return stream_;
    }
private:
    bool initSession();
    void disconnect_i(bool fireEvent);
    void closeSocket();
private:
    friend class ClientSessionHandlerImpl;

    /// 서버에 연결되었을 때 호출된다.
    virtual void onConnected() = 0;

    /// 서버로 부터 접속이 해제되었을 경우 호출된다.
    virtual void onDisconnected() = 0;

    /**
     * 서버로 부터 메세지가 도착하였을 때 호출된다.
     * @internal
     */
    virtual void onMessageArrived(CsMessageType messageType) = 0;
protected:
    // = ACE_Event_Handler overriding
    virtual int handle_input(ACE_HANDLE fd = ACE_INVALID_HANDLE);
    virtual int handle_output(ACE_HANDLE fd = ACE_INVALID_HANDLE);
    //virtual int handle_timeout(const ACE_Time_Value& time, const void *);
    virtual int handle_close(ACE_HANDLE, ACE_Reactor_Mask);
    virtual ACE_HANDLE get_handle() const {
        return stream_.get_handle();
    }
protected: // for Test
    // = MessageBlockProvider overriding
    virtual ACE_Message_Block& acquireSendBlock();
    virtual ACE_Message_Block& acquireRecvBlock();
private:
    bool read();
    bool write();
    void releaseMessageBlocks();
    bool parseHeader();
    bool parseMessage();
    bool isPacketHeaderArrived() const;
    bool isMessageArrived() const;

    int getWriteQueueSize();
private:
    boost::scoped_ptr<SynchMessageBlockManager> messageBlockManager_;
    ACE_Message_Block* recvBlock_;
    ACE_Message_Block* msgBlock_;
    boost::scoped_ptr<PacketCoder> packetCoder_;
    CsPacketHeader headerForReceive_;

    ACE_SOCK_Stream stream_;
    ACE_Message_Queue<ACE_NULL_SYNCH> msgQueue_;
    ACE_Reactor_Notification_Strategy notifier_;

    bool disconnectReserved_;
    bool fireEventAfterFlush_; ///< 전송이 완료된 후 접속해제 이벤트를 호출?

    ACE_Recursive_Thread_Mutex lock_;

    time_t lastLogTime_;
    size_t prevQueueSize_;
};

} // namespace nsrpc

#endif // !defined(NSRPC_REACTORSESSION_H)
