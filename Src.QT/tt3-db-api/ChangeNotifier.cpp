//
//  tt3-db-api/ChangeNotifier.cpp - tt3::db::api::ChangeNotifier class implementation
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
#include "tt3-db-api/API.hpp"
using namespace tt3::db::api;

//////////
//  Construction/destruction
ChangeNotifier::ChangeNotifier()
    :   _pendingNotifications(),
        _workerThread(this)
{
    //  Move signals to this thread to ensure queued
    //  connections to all slots
    moveToThread(&_workerThread);

    //  Go!
    _workerThread.start();
}

ChangeNotifier::~ChangeNotifier()
{
    _pendingNotifications.enqueue(nullptr); //  ...to stop the worker thread
    if (!_workerThread.wait(4 * _WorkerThread::WaitChunkMs))
    {   /// OOPS! Force termination
        _workerThread.terminate();
        _workerThread.wait(ULONG_MAX);
    }
}

//////////
//  Operations
void ChangeNotifier::post(ChangeNotification * notification)
{
    Q_ASSERT(notification != nullptr);

    _pendingNotifications.enqueue(notification);
}

//////////
//  ChangeNotifier::_WorkerThread
void ChangeNotifier::_WorkerThread::run()
{
    //  Go!
    for (; ; )
    {
        ChangeNotification * changeNotification = nullptr;
        if (_changeNotifier->_pendingNotifications.tryDequeue(changeNotification, WaitChunkMs))
        {   //  Handle the pending notification
            if (changeNotification == nullptr)
            {   //  Thread stop requested
                break;
            }
            if (auto databaseClosed =
                dynamic_cast<DatabaseClosedNotification *>(changeNotification))
            {
                emit _changeNotifier->databaseClosed(*databaseClosed);
            }
            else if (auto objectCreated =
                     dynamic_cast<ObjectCreatedNotification *>(changeNotification))
            {
                emit _changeNotifier->objectCreated(*objectCreated);
            }
            else if (auto objectDestroyed =
                     dynamic_cast<ObjectDestroyedNotification *>(changeNotification))
            {
                emit _changeNotifier->objectDestroyed(*objectDestroyed);
            }
            else if (auto objectModified =
                     dynamic_cast<ObjectModifiedNotification *>(changeNotification))
            {
                emit _changeNotifier->objectModified(*objectModified);
            }
            else
            {
                Q_ASSERT(false);
            }
            delete changeNotification;
        }
    }
}

//  End of tt3-db-api/ChangeNotifier.cpp
