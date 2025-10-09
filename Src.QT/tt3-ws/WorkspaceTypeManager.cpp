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

struct WorkspaceTypeManager::_Impl
{
    _Impl()
    {
        for (tt3::db::api::IDatabaseType * databaseType :
             tt3::db::api::DatabaseTypeManager::allDatabaseTypes())
        {
            registry[databaseType] = new WorkspaceTypeImpl(databaseType);
        }
    }

    tt3::util::Mutex                                    guard;
    QMap<tt3::db::api::IDatabaseType*, WorkspaceType>   registry;
};

//////////
//  Operations
WorkspaceType WorkspaceTypeManager::findWorkspaceType(const tt3::util::Mnemonic & mnemonic)
{
    _Impl * impl = _impl();
    tt3::util::Lock lock(impl->guard);

    tt3::db::api::IDatabaseType * databaseType =
        tt3::db::api::DatabaseTypeManager::findDatabaseType(mnemonic);
    return impl->registry.contains(databaseType) ? impl->registry[databaseType] : nullptr;
}

WorkspaceTypes WorkspaceTypeManager::allWorkspaceTypes()
{
    _Impl * impl = _impl();
    tt3::util::Lock lock(impl->guard);

    QList<WorkspaceType> values = impl->registry.values();
    return WorkspaceTypes(values.begin(), values.end());
}

//////////
//  Implementation helpers
WorkspaceTypeManager::_Impl * WorkspaceTypeManager::_impl()
{
    static _Impl impl;
    return &impl;
}

//  End of tt3-ws/WorkspaceTypeManager.cpp
