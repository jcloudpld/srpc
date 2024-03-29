#ifndef NSRPC_MESSAGEBLOCKMANAGER_H
#define NSRPC_MESSAGEBLOCKMANAGER_H

#ifdef _MSC_VER
#  pragma once
#endif

#include "../nsrpc.h"
#include "AceUtil.h"
#if defined(USE_MEMORY_POOL_IN_MESSAGE_BLOCK_MANAGER)
#  include "ObjectAllocator.h"
#endif
#include <srpc/srpc_macros.h>
#include <ace/Message_Block.h>
#ifdef _MSC_VER
#  pragma warning (push)
#  pragma warning (disable: 4127)
#endif
#include <ace/Null_Mutex.h>
#ifdef _MSC_VER
#  pragma warning (pop)
#endif
#include <boost/noncopyable.hpp>
#include <cassert>

namespace nsrpc
{

/**
 * @class MessageBlockManager
 *
 * ACE_Message_Block Manager
 * - pool & cache
 */
template <class Mutex>
class MessageBlockManager : public boost::noncopyable
{
#if defined(USE_MEMORY_POOL_IN_MESSAGE_BLOCK_MANAGER)
protected:
    typedef ObjectAllocator<ACE_Message_Block, Mutex> MessageBlockAllocator;
#endif

public:
    /**
     * ctor
     * @param poolSize 기본 Pool 크기.
     * @param blockSize IO에 사용할 버퍼의 처음 크기
     */
    MessageBlockManager(size_t poolSize, size_t blockSize)
#if defined(USE_MEMORY_POOL_IN_MESSAGE_BLOCK_MANAGER)
        : messageBlockAllocator_(poolSize)
#endif
        { SRPC_UNUSED_ARG(poolSize); SRPC_UNUSED_ARG(blockSize); }

    /// ACE_Message_Block을 요청한다.
    ACE_Message_Block* create(size_t size) {

#if defined(USE_MEMORY_POOL_IN_MESSAGE_BLOCK_MANAGER)

#ifdef _MSC_VER
#  pragma warning (push)
#  pragma warning (disable: 4127)
#endif
        ACE_Message_Block* nb = 0;
        ACE_NEW_MALLOC_RETURN(nb,
            static_cast<ACE_Message_Block*> (
                messageBlockAllocator_.malloc(sizeof(ACE_Message_Block))),
            ACE_Message_Block(size,
                ACE_Message_Block::MB_DATA, 0, 0, 0, 0,
                ACE_DEFAULT_MESSAGE_BLOCK_PRIORITY, ACE_Time_Value::zero,
                ACE_Time_Value::max_time, 0,
                &messageBlockAllocator_),
            0);
        return nb;
#ifdef _MSC_VER
#  pragma warning (pop)
#endif

#else
        return new ACE_Message_Block(size);
#endif
    }

#if defined(USE_MEMORY_POOL_IN_MESSAGE_BLOCK_MANAGER)
protected: // for Test
    MessageBlockAllocator messageBlockAllocator_;
#endif
};


/**
 * @class SynchMessageBlockManager
 * thread-safe MessageBlockManager
 * - forward decl.를 위해 상속함
 */
class SynchMessageBlockManager :
    public MessageBlockManager<Thread_Mutex_With_SpinLock>
{
public:
    SynchMessageBlockManager(size_t poolSize, size_t blockSize) :
        MessageBlockManager<Thread_Mutex_With_SpinLock>(poolSize, blockSize) {}
};


/**
 * @class NoSynchMessageBlockManager
 * thread-nosafe MessageBlockManager
 * - forward decl.를 위해 상속함
 */
class NoSynchMessageBlockManager :
    public MessageBlockManager<ACE_Null_Mutex>
{
public:
    NoSynchMessageBlockManager(size_t poolSize, size_t blockSize) :
        MessageBlockManager<ACE_Null_Mutex>(poolSize, blockSize) {}
};

} // namespace nsrpc

#endif // NSRPC_MESSAGEBLOCKMANAGER_H
