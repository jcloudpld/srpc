#if !defined(NSRPC_CLIENTSESSION_H)
#define NSRPC_CLIENTSESSION_H

#include "../config/config.h"
#include "CsProtocol.h"
#include "MessageBlockProvider.h"
#ifdef _MSC_VER
#  pragma warning( push)
#  pragma warning( disable : 4127 4244 4251 4267 4311 4312 4355)
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
#include <boost/noncopyable.hpp>
#include <string>

class ACE_Reactor;

namespace nsrpc
{

class PacketCoder;
class PacketCoderFactory;
class SynchMessageBlockManager;

/**
 * @class ClientSession
 * Client Session (ACE_Reactor ���)
 * - disable Nagle-algorithm
 * - set non-blocking
 * - set maximum socket buffer size
 */
class NSRPC_API ClientSession :
    public ACE_Event_Handler, protected MessageBlockProvider
{
public:
    /**
     * @reactor Reactor instance.
     * @param packetCoderFactory PacketCoder Factory.
     *          0�̸� ����Ʈ PacketCoder�� ���ȴ�.
     */
    ClientSession(ACE_Reactor* reactor = 0,
        PacketCoderFactory* packetCoderFactory = 0);
    virtual ~ClientSession();

    /**
     * ������ �����մϴ�.
     * @param ip ������ IP �ּ�
     * @param port ������ ��Ʈ ��ȣ
     * @param timeout ���� �ð� ���� (�и��� ����)
     */
    bool connect(const std::string& ip, u_short port, size_t timeout);

    /**
     * ������ ���� ������ �����մϴ�.
     * @param fireEvent disconnected() �̺�Ʈ�� �߻���ų ���ΰ�?
     */
    void disconnect(bool fireEvent = false);

    /**
     * ���� ť�� �ִ� ��� �����͸� ������ �� ������ �����Ѵ�.
     */

    void disconnectGracefully(bool fireEvent = false);

    /**
     * �޼����� �����Ѵ�. ������ ������ ��� ������ �����Ѵ�.
     * @param mblock ������ ����. �ݵ�� acquireSendBlock() ȣ��� �ν��Ͻ���
     *               ȹ���ؾ� �Ѵ�.
     */
    bool sendMessage(ACE_Message_Block& mblock, CsMessageType messageType);

    /// ���� ���ΰ�?
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

    /// ������ ����Ǿ��� �� ȣ��ȴ�.
    virtual void onConnected() = 0;

    /// ������ ���� ������ �����Ǿ��� ��� ȣ��ȴ�.
    virtual void onDisconnected() = 0;

    /**
     * ������ ���� �޼����� �����Ͽ��� �� ȣ��ȴ�.
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
    void prepareRead();
    bool read();
    bool write();
    void releaseMessageBlocks();
    bool parseHeader();
    bool parseMessage();
    bool isMoreDataNeeded() const {
        return neededSize_ > 0;
    }
private:
    boost::scoped_ptr<SynchMessageBlockManager> messageBlockManager_;
    ACE_Message_Block* recvBlock_;
    boost::scoped_ptr<PacketCoder> packetCoder_;
    CsPacketHeader headerForReceive_;

    ACE_SOCK_Stream stream_;
    ACE_Message_Queue<ACE_NULL_SYNCH> msgQueue_;
    ACE_Reactor_Notification_Strategy notifier_;

    bool disconnectReserved_;
    bool fireEventAfterFlush_; ///< ������ �Ϸ�� �� �������� �̺�Ʈ�� ȣ��?

    ACE_Recursive_Thread_Mutex lock_;

    size_t neededSize_;
};

} // namespace nsrpc

#endif // !defined(NSRPC_CLIENTSESSION_H)