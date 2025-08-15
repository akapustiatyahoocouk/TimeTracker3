//
//  tt3-ws/Workspace.cpp - tt3::ws::Workspace class implementation
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
Workspace::Workspace(const WorkspaceAddress & address, tt3::db::api::IDatabase * database)
    :   _address(address),
        _database(database)
{
    Q_ASSERT(_address.isValid());
    Q_ASSERT(_database != nullptr);

    //  Forward database change events
    /*  TODO uncomment & fix the bug
    connect(&_database->changeNotifier(),
            &tt3::db::api::ChangeNotifier::databaseClosed,
            this,
            &Workspace::_onDatabaseClosed,
            Qt::ConnectionType::BlockingQueuedConnection);
    */
}

Workspace::~Workspace()
{
    try
    {
        close();
    }
    catch (...)
    {   //  OOPS! Suppress TODO but log ?
    }
}

//////////
//  Operations (general)
WorkspaceType * Workspace::type() const
{
    return _address.workspaceType();
}

WorkspaceAddress Workspace::address() const
{
    return _address;
}

bool Workspace::isOpen()
{
    tt3::util::Lock lock(_guard);

    return _database != nullptr;
}

void Workspace::close() throws(WorkspaceException)
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
//  Implementation helpers
void Workspace::_markClosed()
{
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
void Workspace::_onDatabaseClosed(tt3::db::api::DatabaseClosedNotification notification)
{
    qDebug() << "Workspace::_onDatabaseClosed()";
    Q_ASSERT(notification.database == _database);
}

//  End of tt3-ws/Workspace.cpp
