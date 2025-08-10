//
//  tt3-ws/WorkspaceType.cpp - tt3::ws::WorkspaceType class implementation
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
WorkspaceType::WorkspaceType(tt3::db::api::IDatabaseType * databaseType)
    :   _databaseType(databaseType)
{
    Q_ASSERT(_databaseType != nullptr);
}

WorkspaceType::~WorkspaceType()
{
}

//////////
//  Operation (general)
QString WorkspaceType::mnemonic() const
{
    return _databaseType->mnemonic();
}

QString WorkspaceType::displayName() const
{
    return _databaseType->displayName();
}

QIcon WorkspaceType::smallIcon() const
{
    return _databaseType->smallIcon();
}

QIcon WorkspaceType::largeIcon() const
{
    return _databaseType->largeIcon();
}

bool WorkspaceType::isOperational() const
{
    return _databaseType->isOperational();
}

QString WorkspaceType::shortStatusReport() const
{
    return _databaseType->shortStatusReport();
}

QString WorkspaceType::fullStatusReport() const
{
    return _databaseType->fullStatusReport();
}

//////////
//  Operations (address handling)
WorkspaceAddress WorkspaceType::defaultWorkspaceAddress() const
{
    return WorkspaceAddress(_databaseType->defaultDatabaseAddress());
}

WorkspaceAddress WorkspaceType::enterNewWorkspaceAddress(QWidget * parent)
{
    tt3::db::api::IDatabaseAddress * databaseAddress =
        _databaseType->enterNewDatabaseAddress(parent);
    return WorkspaceAddress(databaseAddress);
}

WorkspaceAddress WorkspaceType::enterExistingWorkspaceAddress(QWidget * parent)
{
    tt3::db::api::IDatabaseAddress * databaseAddress =
        _databaseType->enterExistingDatabaseAddress(parent);
    return WorkspaceAddress(databaseAddress);
}

WorkspaceAddress WorkspaceType::parseWorkspaceAddress(const QString & externalForm) throws(WorkspaceException)
{
    tt3::db::api::IDatabaseAddress * databaseAddress =
        _databaseType->parseDatabaseAddress(externalForm);
    //  TODO catch & re-throw tt3::db::api::DatabaseException!!!
    return WorkspaceAddress(databaseAddress);
}

//  End of tt3-ws/WorkspaceType.cpp
