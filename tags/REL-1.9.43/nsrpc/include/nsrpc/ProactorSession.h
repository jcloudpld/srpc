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
 * - CAUTION: �ݵ�� recvLock_�� �� �� sendLock_�� �ɾ�� �Ѵ�
 */
class NSRPC_API ProactorSession : public Session,
    public NSRPC_Service_Handler, protected MessageBlockProvider
{
public:
    /**
     * @struct Stats
     * ������ ��� ������ ����Ѵ�.
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
    /// remote host�� �����Ѵ�(ACE_Socket_Connector �̿�)
    bool connect(const srpc::String& host, u_short port, size_t timeout);

    /**
     * �޼����� �����Ѵ�. ������ ������ ��� ������ �����Ѵ�.
     * @param mblock ������ ����. �ݵ�� acquireSendBlock() ȣ��� �ν��Ͻ���
     *               ȹ���ؾ� �Ѵ�.
     * @param messageType �޼��� Ÿ��
     */
    void sendMessage(ACE_Message_Block& mblock,
        CsMessageType messageType = mtGeneral);

    /// AIO�� ����Ѵ�. AIO ������ ������ ������ �����ϰ� �Ѵ�.
    void disconnect();

    /**
     * ������ �Ϸ��� �� ������ �����Ѵ�
     * - Ŭ���̾�Ʈ�� FIN�� ���� �� ������ �������� ������, ������ ����������
     *   �� �� �����Ƿ� ������ �������� Ÿ�̸Ӹ� �ּ� ������ ������ �����Ѵ�.
     */
    void disconnectGracefully();

    /// ���� ���ΰ�?
    bool isConnected() const;

    /// ��� ������ ��´�.
    const Stats& getStats() const {
        return stats_;
    }

    /// Ŭ���̾�Ʈ �����̸� Ŭ���̾�Ʈ�� IP �ּ�
    const ACE_INET_Addr& getRemoteAddress() const {
        return remoteAddress_;
    }

    /// Ŭ���̾�Ʈ �����̸� ���� ���� ���ε��� IP �ּ�
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
    /// ���ӵ� ��� ȣ��ȴ�.
    /// @return false�̸� ������ �����Ѵ�
    virtual bool onConnected() {
        return true;
    }
private:
    /// Ŭ���̾�Ʈ�� ���� �޼����� �����Ͽ���.
    virtual bool onMessageArrived(CsMessageType messageType) = 0;
    virtual void onThrottling(size_t readBytes, size_t maxBytesPerSecond);
    /// ������ ������ ��� ȣ��ȴ�.
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
