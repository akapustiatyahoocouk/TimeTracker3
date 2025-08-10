//
//  tt3-ws/WorkspaceTypeRegistry.cpp - tt3::ws::WorkspaceTypeRegistry class implementation
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

QMap<tt3::db::api::IDatabaseType*, WorkspaceType*> WorkspaceTypeRegistry::_registry;

//////////
//  Operations
WorkspaceType * WorkspaceTypeRegistry::findWorkspaceType(const QString & mnemonic)
{   //  TODO synchronize ?
    _collectWorkspaceTypes();
    tt3::db::api::IDatabaseType * databaseType =
        tt3::db::api::DatabaseTypeRegistry::findDatabaseType(mnemonic);
    return _registry.contains(databaseType) ? _registry[databaseType] : nullptr;
}

WorkspaceTypes WorkspaceTypeRegistry::allWorkspaceTypes()
{   //  TODO synchronize ?
    _collectWorkspaceTypes();
    return tt3::util::toSet(_registry.values());
}

//////////
//  Implementation helpers
void WorkspaceTypeRegistry::_collectWorkspaceTypes()
{
    //  TODO assert synchronized
    if (_registry.isEmpty())
    {
        for (tt3::db::api::IDatabaseType * databaseType :
             tt3::db::api::DatabaseTypeRegistry::allDatabaseTypes())
        {
            _registry[databaseType] = new WorkspaceType(databaseType);
        }
    }
}

WorkspaceType * WorkspaceTypeRegistry::_findWorkspaceType(tt3::db::api::IDatabaseType * databaseType)
{   //  TODO assert synchronized
    _collectWorkspaceTypes();
    return _registry.contains(databaseType) ? _registry[databaseType] : nullptr;
}

//  End of tt3-ws/WorkspaceTypeRegistry.cpp
