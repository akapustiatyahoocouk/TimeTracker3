//
//  tt3-ws/WorkspaceTypeManager.cpp - tt3::ws::WorkspaceTypeManager class implementation
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

tt3::util::Mutex WorkspaceTypeManager::_guard;
QMap<tt3::db::api::IDatabaseType*, WorkspaceType> WorkspaceTypeManager::_registry;

//////////
//  Operations
WorkspaceType WorkspaceTypeManager::findWorkspaceType(const QString & mnemonic)
{
    tt3::util::Lock lock(_guard);

    _collectWorkspaceTypes();
    tt3::db::api::IDatabaseType * databaseType =
        tt3::db::api::DatabaseTypeManager::findDatabaseType(mnemonic);
    return _registry.contains(databaseType) ? _registry[databaseType] : nullptr;
}

WorkspaceTypes WorkspaceTypeManager::allWorkspaceTypes()
{
    tt3::util::Lock lock(_guard);

    _collectWorkspaceTypes();
    QList<WorkspaceType> values = _registry.values();
    return WorkspaceTypes(values.begin(), values.end());
}

//////////
//  Implementation helpers
void WorkspaceTypeManager::_collectWorkspaceTypes()
{
    Q_ASSERT(_guard.isLockedByCurrentThread());

    if (_registry.isEmpty())
    {
        for (tt3::db::api::IDatabaseType * databaseType :
             tt3::db::api::DatabaseTypeManager::allDatabaseTypes())
        {
            _registry[databaseType] = new WorkspaceTypeImpl(databaseType);
        }
    }
}

WorkspaceType WorkspaceTypeManager::_findWorkspaceType(tt3::db::api::IDatabaseType * databaseType)
{
    Q_ASSERT(_guard.isLockedByCurrentThread());

    _collectWorkspaceTypes();
    return _registry.contains(databaseType) ? _registry[databaseType] : nullptr;
}

//  End of tt3-ws/WorkspaceTypeManager.cpp
