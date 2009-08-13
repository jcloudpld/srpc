#ifndef NSRPC_ASYNCH_RW_H
#define NSRPC_ASYNCH_RW_H

#ifdef _MSC_VER
#  pragma once
#endif

#include "../utility/AceUtil.h"
#include "../utility/Logger.h"
#include "../config/Proactor.h"
#ifdef _MSC_VER
#  pragma warning (push)
#  pragma warning (disable: 4127 4244 4251 4267 4312 4996)
#endif
#if defined (NSRPC_USE_TPROACTOR)
#  include <TProactor/Asynch_IO.h>
#else
#  include <ace/Asynch_IO.h>
#endif // NSRPC_USE_TPROACTOR
#ifdef _MSC_VER
#  pragma warning (pop)
#endif

// stolen from TProactor test

namespace nsrpc
{

/** @addtogroup proactor
* @{
*/

/**
 * @class Asynch_RW_T
 */
template <class READER, class WRITER>
class Asynch_RW_T
{
public:
    Asynch_RW_T() {
        reset();
    }

    ~Asynch_RW_T() {
        cancel();
        close();
    }

    /**
     * Initializes the factory with information which will be used with
     * each asynchronous call. If (<handle> == ACE_INVALID_HANDLE),
     * <NSRPC_Handler::handle> will be called on the <handler> to get the
     * correct handle.
     */
    bool open(NSRPC_Handler& handler, ACE_HANDLE handle = ACE_INVALID_HANDLE,
        const void* completionKey = 0, NSRPC_Proactor* proactor = 0,
        bool passOwnership = false) {
        if (cancelled_) {
            return false;
        }
        handle_ = handle;
        completionKey_ = completionKey;
        hasOwnership_ = passOwnership;

        if (reader_.open(handler, handle, completionKey, proactor) < 0) {
            return false;
        }
        if (writer_.open(handler, handle, completionKey, proactor) < 0) {
            return false;
        }

        return true;
    }

    void cancel() {
        if (! cancelled_) {
            cancelled_ = true;
            if (isOpened()) {
                (void)reader_.cancel();
                (void)writer_.cancel();
            }
        }
    }

    void shutdown() {
        ACE_OS::shutdown(handle_, ACE_SHUTDOWN_WRITE);
    }

    void close() {
        if (isOpened()) {
            if (hasOwnership_) {
                ACE_OS::closesocket(handle_);
            }
            handle_ = ACE_INVALID_HANDLE;
        }
    }

    int shouldFinish() const {
        return (! isOpened()) || cancelled_;
    }

    void reset() {
        handle_ = ACE_INVALID_HANDLE;
        hasOwnership_ = false;
        cancelled_ = false;
        completionKey_ = 0;
    }

    READER& reader() {
        return reader_;
    }
    
    WRITER& writer() {
        return writer_;
    }

    ACE_HANDLE getHandle() const {
        return handle_;
    }

    bool cancelled() const {
        return cancelled_;
    }

    bool isOpened() const {
        return isValidHandle(handle_);
    }
protected:
    ACE_HANDLE handle_;
    bool hasOwnership_; 
    volatile bool cancelled_;

    const void* completionKey_;
    READER reader_;
    WRITER writer_;
};


/**
 * @class Asynch_RW_Stream_T
 */
template <class READER, class WRITER>
class Asynch_RW_Stream_T : public Asynch_RW_T<READER, WRITER>
{
public:
    int read(ACE_Message_Block& messageBlock, size_t bytesToRead,
        const void* act = 0, int priority = 0,
        int signalNumber = ACE_SIGRTMIN) {
        if (this->shouldFinish()) {
            return -1;
        }
        return this->reader().read(messageBlock, bytesToRead, act, priority,
            signalNumber);
    }

    int write(ACE_Message_Block& messageBlock, size_t bytesToWrite,
        const void* act = 0, int priority = 0,
        int signalNumber = ACE_SIGRTMIN) {
        if (this->shouldFinish()) {
            return -1;
        }
        return this->writer().write(messageBlock, bytesToWrite, act, priority,
            signalNumber);
    }
};


/**
 * @class Asynch_RW_Dgram_T
 */
template <class READER, class WRITER>
class Asynch_RW_Dgram_T : public Asynch_RW_T<READER, WRITER>
{
public:
    ssize_t recv(ACE_Message_Block* messageBlock,
        size_t& bytesReceived, int flags, int protocol_family = PF_INET,
        const void* act = 0, int priority = 0,
        int signalNumber = ACE_SIGRTMIN) {
        if (this->shouldFinish()) {
            return -1;
        }
        return this->reader().recv(messageBlock, bytesReceived, flags,  
            protocol_family, act, priority, signalNumber);
    }

    ssize_t send(ACE_Message_Block* messageBlock, size_t& bytesSent,
        int flags, const ACE_Addr& remoteAddr, const void* act = 0,
        int priority = 0, int signalNumber = ACE_SIGRTMIN) {
        if (this->shouldFinish()) {
            return -1;
        }
        return this->writer().send(messageBlock, bytesSent, flags, remoteAddr,
            act, priority, signalNumber);
    }
};

// = Typedefs
  
/// @class Asynch_RW_Stream
class Asynch_RW_Stream :
    public Asynch_RW_Stream_T<
        NSRPC_Asynch_Read_Stream, NSRPC_Asynch_Write_Stream> 
{};


/// @class Asynch_RW_Dgram
class Asynch_RW_Dgram :
    public Asynch_RW_Dgram_T<
        NSRPC_Asynch_Read_Dgram, NSRPC_Asynch_Write_Dgram> 
{};

/** @} */ // addtogroup proactor

} // namespace nsrpc

#endif // NSRPC_ASYNCH_RW_H
