//
//  tt3-db-api/DatabaseTypeManager.cpp tt3::db::api::DatabaseTypeManager class implementation
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

struct DatabaseTypeManager::_Impl
{
    tt3::util::Mutex                            guard;
    QMap<tt3::util::Mnemonic, IDatabaseType*>   registry;
};

//////////
//  Operations
bool DatabaseTypeManager::registerDatabaseType(IDatabaseType * databaseType)
{
    Q_ASSERT(databaseType != nullptr);

    _Impl * impl = _impl();
    tt3::util::Lock _(impl->guard);

    if (impl->registry.contains(databaseType->mnemonic()))
    {
        return databaseType == impl->registry[databaseType->mnemonic()];
    }
    impl->registry[databaseType->mnemonic()] = databaseType;
    return true;
}

bool DatabaseTypeManager::unregisterDatabaseType(
        IDatabaseType * databaseType
    )
{
    Q_ASSERT(databaseType != nullptr);

    _Impl * impl = _impl();
    tt3::util::Lock _(impl->guard);

    if (impl->registry.contains(databaseType->mnemonic()))
    {
        impl->registry.remove(databaseType->mnemonic());
        return true;
    }
    return false;
}

IDatabaseType * DatabaseTypeManager::findDatabaseType(const tt3::util::Mnemonic & mnemonic)
{
    _Impl * impl = _impl();
    tt3::util::Lock _(impl->guard);

    return impl->registry.contains(mnemonic) ? impl->registry[mnemonic] : nullptr;
}

DatabaseTypes DatabaseTypeManager::allDatabaseTypes()
{
    _Impl * impl = _impl();
    tt3::util::Lock _(impl->guard);

    QList<IDatabaseType*> values = impl->registry.values();
    return DatabaseTypes(values.cbegin(), values.cend());
}

//////////
//  Implementation helpers
DatabaseTypeManager::_Impl * DatabaseTypeManager::_impl()
{
    static _Impl impl;
    return &impl;
}

//  End of tt3-db-api/DatabaseTypeManager.cpp
