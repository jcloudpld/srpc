#ifndef NSRPC_SESSION_H
#define NSRPC_SESSION_H

#include "config/config.h"
#include "config/Proactor.h"
#include "detail/Asynch_RW.h"
#include "detail/MessageBlockProvider.h"
#include "detail/CsProtocol.h"
#include <srpc/StringTypes.h>
#include <ace/INET_Addr.h>
#include <boost/scoped_ptr.hpp>

namespace nsrpc
{

class SessionDestroyer;
class PacketCoder;
class SynchMessageBlockManager;

/** @addtogroup session
* @{
*/

/**
 * @class Session
 *
 * Asynchronous Session
 * - CAUTION: �ݵ�� recvLock_�� �� �� sendLock_�� �ɾ�� �Ѵ�
 */
class NSRPC_API Session :
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

        Stats() {
            reset();
        }

        void reset() {
            recvBytes_ = 0;
            sentBytes_ = 0;
        }
    };
public:
    /**
     * ctor
     * @param sessionDestroyer ���� ������ �ν��Ͻ�.
     * @param packetCoder PacketCoder �ν��Ͻ�.
     *          �޸� �������� �Ѱܾ� �Ѵ�(���� �Ҵ�).
     * @param messageBlockManager �޸� ���� ������.
     * @param proactor Proactor �ν��Ͻ�. 0�̸� �̱����� ���ȴ�.
     */
    Session(SessionDestroyer& sessionDestroyer,
        PacketCoder* packetCoder,
        SynchMessageBlockManager& messageBlockManager,
        NSRPC_Proactor* proactor = 0);
    virtual ~Session();
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
    bool isConnected() const {
        return isValidHandle(stream_.getHandle());
    }

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
    virtual void onConnected() {}
private:
    /// Ŭ���̾�Ʈ�� ���� �޼����� �����Ͽ���.
    virtual bool onMessageArrived(CsMessageType messageType) = 0;
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
    bool readMessageBody(size_t neededBytes);
    bool read(size_t neededBytes);

    bool write(ACE_Message_Block& mblock);

    void reset();

    void startDisconnectTimer();
    void stopDisconnectTimer();

    bool isSafeToDelete() const {
        return (pendingReadCount_ <= 0) && (pendingWriteCount_ <= 0);
    }
private:
    SessionDestroyer& sessionDestroyer_;
    NSRPC_Proactor* proactor_;
    boost::scoped_ptr<PacketCoder> packetCoder_;
    CsPacketHeader headerForReceive_;

    SynchMessageBlockManager& messageBlockManager_;
    ACE_Message_Block* recvBlock_;

    Asynch_RW_Stream stream_;

    ACE_INET_Addr remoteAddress_;
    ACE_INET_Addr localAddress_;

    ACE_Atomic_Op<ACE_Thread_Mutex, long> pendingReadCount_;
    ACE_Atomic_Op<ACE_Thread_Mutex, long> pendingWriteCount_;

    long disconnectTimer_;
    bool disconnectReserved_;

    Stats stats_;

    ACE_Thread_Mutex sendLock_;
    ACE_Thread_Mutex recvLock_;
};

/** @} */ // addtogroup session

} // namespace nsrpc

#endif // NSRPC_SESSION_H