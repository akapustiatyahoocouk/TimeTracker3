//
//  tt3-util/Mutex.cpp - the tt3::til::Mutex class implementation
//
//  TimeTracker3
//  Copyright (C) 2026, Andrey Kapustin
//
//  This program is free software: you can redistribute it and/or modify
//  it under the terms of the GNU General Public License as published by
//  the Free Software Foundation, either version 3 of the License, or
//  (at your option) any later version.
//
//  This program is distributed in the hope that it will be useful,
//  but WITHOUT ANY WARRANTY; without even the implied warranty of
//  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
//  GNU General Public License for more details.
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
