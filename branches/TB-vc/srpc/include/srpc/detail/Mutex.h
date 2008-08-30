#ifndef SRPC_MUTEX_H
#define SRPC_MUTEX_H

#include <boost/noncopyable.hpp>
#include <boost/config.hpp>  // for workarounds

// boost::pool 라이브러리의 mutex.hpp에서 코드를 훔쳐와서 정리했음.

// Extremely Light-Weight wrapper classes for OS thread synchronization
// Configuration: for now, we just choose between pthread or Win32 mutexes or
//                none

#define SRPC_MUTEX_HELPER_NONE         0
#define SRPC_MUTEX_HELPER_WIN32        1
#define SRPC_MUTEX_HELPER_PTHREAD      2

#if !defined(BOOST_HAS_THREADS) && !defined(BOOST_NO_MT)
#  define BOOST_NO_MT
#endif

#ifdef BOOST_NO_MT
    // No multithreading -> make locks into no-ops
#  define SRPC_MUTEX_HELPER SRPC_MUTEX_HELPER_NONE
#else
#  ifdef BOOST_WINDOWS
#    define SRPC_MUTEX_HELPER SRPC_MUTEX_HELPER_WIN32
#  else
#    include <unistd.h>
#  ifdef _POSIX_THREADS
#    define SRPC_MUTEX_HELPER SRPC_MUTEX_HELPER_PTHREAD
#  endif
#endif
#endif

#ifndef SRPC_MUTEX_HELPER
#  error Unable to determine platform mutex type; define BOOST_NO_MT to assume single-threaded
#endif

#ifndef BOOST_NO_MT
#  ifdef BOOST_WINDOWS
#    include <windows.h>
#  endif
#  ifdef _POSIX_THREADS
#    include <pthread.h>
#  endif
#endif

namespace srpc {

#ifndef BOOST_NO_MT

#ifdef BOOST_WINDOWS

class Win32Mutex : public boost::noncopyable
{
private:
    CRITICAL_SECTION mutex;
public:
    Win32Mutex() {
        InitializeCriticalSection(&mutex);
    }

    ~Win32Mutex() {
        DeleteCriticalSection(&mutex);
    }

    void lock() {
        EnterCriticalSection(&mutex);
    }

    void unlock() {
        LeaveCriticalSection(&mutex);
    }
};

#endif // defined(BOOST_WINDOWS)

#ifdef _POSIX_THREADS

class PthreadMutex : public boost::noncopyable
{
private:
    pthread_mutex_t mutex;
public:
    PthreadMutex() {
        pthread_mutex_init(&mutex, 0);
    }

    ~PthreadMutex() {
        pthread_mutex_destroy(&mutex);
    }

    void lock() {
        pthread_mutex_lock(&mutex);
    }

    void unlock() {
        pthread_mutex_unlock(&mutex);
    }
};

#endif // defined(_POSIX_THREADS)

#endif // !defined(BOOST_NO_MT)

class NullMutex : public boost::noncopyable
{
public:
    NullMutex() {}

    void lock() {}
    void unlock() {}
};

#if SRPC_MUTEX_HELPER == SRPC_MUTEX_HELPER_NONE
typedef NullMutex LightMutex;
#elif SRPC_MUTEX_HELPER == SRPC_MUTEX_HELPER_WIN32
typedef Win32Mutex LightMutex;
#elif SRPC_MUTEX_HELPER == SRPC_MUTEX_HELPER_PTHREAD
typedef PthreadMutex LightMutex;
#endif

#undef SRPC_MUTEX_HELPER_WIN32
#undef SRPC_MUTEX_HELPER_PTHREAD
#undef SRPC_MUTEX_HELPER_NONE
#undef SRPC_MUTEX_HELPER

template <typename Mutex>
class ScopedLock : public boost::noncopyable
{
private:
    Mutex& mutex_;
public:
    explicit ScopedLock(Mutex & mutex) :
        mutex_(mutex) {
        mutex_.lock();
    }

    ~ScopedLock() {
        mutex_.unlock();
    }
};

} // namespace srpc

#endif // SRPC_MUTEX_H
