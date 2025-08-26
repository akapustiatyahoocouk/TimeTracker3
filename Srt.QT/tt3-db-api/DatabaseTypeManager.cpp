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

tt3::util::Mutex DatabaseTypeManager::_guard;
QMap<QString, IDatabaseType*> DatabaseTypeManager::_registry;

//////////
//  Operations
bool DatabaseTypeManager::registerDatabaseType(IDatabaseType * databaseType)
{
    Q_ASSERT(databaseType != nullptr);

    tt3::util::Lock oock(_guard);

    IDatabaseType * registeredDatabaseType = findDatabaseType(databaseType->mnemonic());
    if (registeredDatabaseType == nullptr)
    {
        _registry[databaseType->mnemonic()] = databaseType;
        return true;
    }
    return databaseType == registeredDatabaseType;
}

IDatabaseType * DatabaseTypeManager::findDatabaseType(const QString & mnemonic)
{
    tt3::util::Lock oock(_guard);

    return _registry.contains(mnemonic) ? _registry[mnemonic] : nullptr;
}

DatabaseTypes DatabaseTypeManager::allDatabaseTypes()
{
    tt3::util::Lock oock(_guard);

    QList<IDatabaseType*> values = _registry.values();
    return DatabaseTypes(values.begin(), values.end());
}

//  End of tt3-db-api/DatabaseTypeManager.cpp
