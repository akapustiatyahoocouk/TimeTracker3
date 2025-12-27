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
        refreshRegistry();
    }

    using Registry = QMap<tt3::db::api::IDatabaseType*, WorkspaceType>;

    tt3::util::Mutex    guard;
    Registry            registry;

    void        refreshRegistry()
    {
        auto newDatabaseTypes = tt3::db::api::DatabaseTypeManager::all();
        //  Add new entries...
        for (auto databaseType : newDatabaseTypes)
        {
            if (!registry.contains(databaseType))
            {
                registry[databaseType] = new WorkspaceTypeImpl(databaseType);
            }
        }
        //  ...ad delete stale ones
        for (auto databaseType : registry.keys())
        {
            if (!newDatabaseTypes.contains(databaseType))
            {
                delete registry[databaseType];
                registry.remove(databaseType);
            }
        }
    }
};

//////////
//  Operations
WorkspaceType WorkspaceTypeManager::find(const tt3::util::Mnemonic & mnemonic)
{
    _Impl * impl = _impl();
    tt3::util::Lock _(impl->guard);

    impl->refreshRegistry();
    tt3::db::api::IDatabaseType * databaseType =
        tt3::db::api::DatabaseTypeManager::find(mnemonic);
    return impl->registry.contains(databaseType) ? impl->registry[databaseType] : nullptr;
}

WorkspaceTypes WorkspaceTypeManager::all()
{
    _Impl * impl = _impl();
    tt3::util::Lock _(impl->guard);

    impl->refreshRegistry();
    QList<WorkspaceType> values = impl->registry.values();
    return WorkspaceTypes(values.cbegin(), values.cend());
}

//////////
//  Implementation helpers
WorkspaceTypeManager::_Impl * WorkspaceTypeManager::_impl()
{
    static _Impl impl;
    return &impl;
}

//  End of tt3-ws/WorkspaceTypeManager.cpp
