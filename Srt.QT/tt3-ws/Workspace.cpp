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
}

Workspace::~Workspace()
{
    delete _database;   //  "delete nullptr" is safe
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
            delete _database;
            _database = nullptr;
        }
        catch (...)
        {   //  Ensure cleanup on throw
            delete _database;
            _database = nullptr;
        }
    }
}


//  End of tt3-ws/Workspace.cpp
