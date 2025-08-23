//
//  tt3-ws/WorkspaceImpl.cpp - tt3::ws::WorkspaceImpl class implementation
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
#include "tt3-ws/API.hpp"
using namespace tt3::ws;

//////////
//  Construction/destruction - from friends only
WorkspaceImpl::WorkspaceImpl(const WorkspaceAddress & address, tt3::db::api::IDatabase * database)
    :   _address(address),
        _database(database)
{
    Q_ASSERT(_address != nullptr);
    Q_ASSERT(_database != nullptr);

    //  Forward database change events
    connect(&_database->changeNotifier(),
            &tt3::db::api::ChangeNotifier::databaseClosed,
            this,
            &WorkspaceImpl::_onDatabaseClosed);
}

WorkspaceImpl::~WorkspaceImpl()
{
    try
    {
        close();
    }
    catch (...)
    {   //  OOPS! Suppress TODO but log ?
    }
    delete _accessCacheKey; //  "delete nullptr" is safe
}

//////////
//  Operations (general)
WorkspaceType WorkspaceImpl::type() const
{
    return _address->workspaceType();
}

WorkspaceAddress WorkspaceImpl::address() const
{
    return _address;
}

bool WorkspaceImpl::isOpen()
{
    tt3::util::Lock lock(_guard);

    return _database != nullptr;
}

void WorkspaceImpl::close() throws(WorkspaceException)
{
    tt3::util::Lock lock(_guard);

    if (_database != nullptr)
    {
        try
        {
            _database->close(); //  may throw
            //  TODO translate & re-throw WorkspaceException
            _markClosed();
        }
        catch (...)
        {   //  Ensure cleanup on throw
            _markClosed();
            throw;
        }
    }
}

//////////
//  Operations (access control)
bool WorkspaceImpl::canAccess(const Credentials & credentials) const throws(WorkspaceException)
{
    tt3::util::Lock lock(_guard);
    _ensureOpen();

    return _database->tryLogin(credentials.login(), credentials._password) != nullptr;
}

//////////
//  Implementation helpers
void WorkspaceImpl::_ensureOpen() const throws(WorkspaceException)
{
    Q_ASSERT(_guard.isLockedByCurrentThread());

    if (_database == nullptr)
    {
        throw WorkspaceClosedException();
    }
}

void WorkspaceImpl::_markClosed()
{
    Q_ASSERT(_guard.isLockedByCurrentThread());
    Q_ASSERT(_database != nullptr);

    delete _database;
    _database = nullptr;
    //  The "database closed" notification fro m the
    //  database will be missed, as database (along with
    //  its change notifier) no longer existsm so we
    //  need to fake a "workspace closed" signal
    //  TODO uncomment & fix the bug emit workspaceClosed(WorkspacePtr(this));
}

//////////
//  Event handlers
void WorkspaceImpl::_onDatabaseClosed(tt3::db::api::DatabaseClosedNotification notification)
{
    qDebug() << "Workspace::_onDatabaseClosed()";
    Q_ASSERT(notification.database == _database);
}

//  End of tt3-ws/WorkspaceImpl.cpp
