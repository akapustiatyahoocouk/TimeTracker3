//
//  tt3-util/Mutex.cpp - the tt3::til::Mutex class implementation
//
//////////
#include "tt3-util/API.hpp"
using namespace tt3::util;

//////////
//  Operations
void Mutex::lock()
{
    _impl.lock();

    //  Update extended mutex state
    QMutexLocker lock(&_extendedStateGuard);
    _lockCount++;
    _lockingThread = QThread::currentThread();
}

bool Mutex::tryLock(int timeoutMs)
{
    if (!_impl.tryLock(timeoutMs))
    {
        return false;
    }

    //  Update extended mutex state
    QMutexLocker lock(&_extendedStateGuard);
    _lockCount++;
    _lockingThread = QThread::currentThread();

    return true;
}

void Mutex::unlock()
{
    //  Update extended mutex state
    QMutexLocker lock(&_extendedStateGuard);
    if (_lockCount > 0 && _lockingThread == QThread::currentThread())
    {   //  Can do!
        _impl.unlock();
        if (--_lockCount == 0)
        {
            _lockingThread = nullptr;
        }
    }
    else
    {   //  ...else the current thread has no business 'unlock'ing this mutex
        Q_ASSERT(false);
    }
}

bool Mutex::isLockedBy(QThread * thread)
{
    Q_ASSERT(thread != nullptr);
    QMutexLocker lock(&_extendedStateGuard);
    return _lockCount > 0 && _lockingThread == thread;
}

bool Mutex::isLockedByCurrentThread()
{
    QMutexLocker lock(&_extendedStateGuard);
    return _lockCount > 0 && _lockingThread == QThread::currentThread();
}

//  End of tt3-util/Mutex.cpp
