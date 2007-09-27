#ifndef NSRPC_MESSAGEBLOCKMANAGER_H
#define NSRPC_MESSAGEBLOCKMANAGER_H

#include "ObjectAllocator.h"
#include "VariousMemoryAllocator.h"
#include <ace/Message_Block.h>
#include <ace/Thread_Mutex.h>
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
protected:
    typedef VariousMemoryAllocator<Mutex> BufferAllocator;
    typedef ObjectAllocator<ACE_Message_Block, Mutex> MessageBlockAllocator;
    typedef ObjectAllocator<ACE_Data_Block, Mutex> DataBlockAllocator;
public:
    /**
     * ctor
     * @param poolSize 기본 Pool 크기.
     * @param blockSize IO에 사용할 버퍼의 처음 크기
     */
    MessageBlockManager(size_t poolSize, size_t blockSize) :
        messageBlockAllocator_(poolSize),
        dataBlockAllocator_(poolSize),
        bufferAllocator_(poolSize, blockSize) {}

    /// ACE_Message_Block을 요청한다.
    ACE_Message_Block* create(size_t size) {
        ACE_Message_Block* nb = 0;
        ACE_NEW_MALLOC_RETURN(nb,
            static_cast<ACE_Message_Block*> (
                messageBlockAllocator_.malloc(sizeof(ACE_Message_Block))),
            ACE_Message_Block(size, ACE_Message_Block::MB_DATA, 0, 0,
                &bufferAllocator_,
                0, ACE_DEFAULT_MESSAGE_BLOCK_PRIORITY, ACE_Time_Value::zero,
                ACE_Time_Value::max_time, &dataBlockAllocator_,
                &messageBlockAllocator_),
            0);
        return nb;
    }
protected: // for Test
    MessageBlockAllocator messageBlockAllocator_;
    DataBlockAllocator dataBlockAllocator_;
    BufferAllocator bufferAllocator_;
};


/**
 * @class SynchMessageBlockManager
 * thread-safe MessageBlockManager
 * - forward decl.를 위해 상속함
 */
class SynchMessageBlockManager : public MessageBlockManager<ACE_Thread_Mutex>
{
public:
    SynchMessageBlockManager(size_t poolSize, size_t blockSize) :
        MessageBlockManager<ACE_Thread_Mutex>(poolSize, blockSize) {}
};


/**
 * @class NoSynchMessageBlockManager
 * thread-nosafe MessageBlockManager
 * - forward decl.를 위해 상속함
 */
class NoSynchMessageBlockManager : public MessageBlockManager<ACE_Null_Mutex>
{
public:
    NoSynchMessageBlockManager(size_t poolSize, size_t blockSize) :
      MessageBlockManager<ACE_Null_Mutex>(poolSize, blockSize) {}
};

} // namespace nsrpc

#endif // NSRPC_MESSAGEBLOCKMANAGER_H
