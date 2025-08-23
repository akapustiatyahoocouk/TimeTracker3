//
//  tt3-ws/WorkspaceTypeImpl.cpp - tt3::ws::WorkspaceType class implementation
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
WorkspaceTypeImpl::WorkspaceTypeImpl(tt3::db::api::IDatabaseType * databaseType)
    :   _databaseType(databaseType),
        _validator(databaseType->validator())
{
    Q_ASSERT(_databaseType != nullptr);
}

WorkspaceTypeImpl::~WorkspaceTypeImpl()
{
}

//////////
//  Operation (general)
QString WorkspaceTypeImpl::mnemonic() const
{
    return _databaseType->mnemonic();
}

QString WorkspaceTypeImpl::displayName() const
{
    return _databaseType->displayName();
}

QIcon WorkspaceTypeImpl::smallIcon() const
{
    return _databaseType->smallIcon();
}

QIcon WorkspaceTypeImpl::largeIcon() const
{
    return _databaseType->largeIcon();
}

bool WorkspaceTypeImpl::isOperational() const
{
    return _databaseType->isOperational();
}

QString WorkspaceTypeImpl::shortStatusReport() const
{
    return _databaseType->shortStatusReport();
}

QString WorkspaceTypeImpl::fullStatusReport() const
{
    return _databaseType->fullStatusReport();
}

//////////
//  Operations (address handling)
WorkspaceAddress WorkspaceTypeImpl::defaultWorkspaceAddress() const
{
    tt3::db::api::IDatabaseAddress * databaseAddress =
        _databaseType->defaultDatabaseAddress();    //  can return nullptr
    return (databaseAddress != nullptr) ?
        WorkspaceAddress(
            _mapDatabaseAddress(databaseAddress),
            [](WorkspaceAddressImpl * p) { delete p; }) :
        WorkspaceAddress();
}

WorkspaceAddress WorkspaceTypeImpl::enterNewWorkspaceAddress(QWidget * parent)
{
    tt3::db::api::IDatabaseAddress * databaseAddress =
        _databaseType->enterNewDatabaseAddress(parent); //  can return nullptr
    return (databaseAddress != nullptr) ?
        WorkspaceAddress(
            _mapDatabaseAddress(databaseAddress),
            [](WorkspaceAddressImpl * p) { delete p; }) :
        WorkspaceAddress();
}

WorkspaceAddress WorkspaceTypeImpl::enterExistingWorkspaceAddress(QWidget * parent)
{
    tt3::db::api::IDatabaseAddress * databaseAddress =
        _databaseType->enterExistingDatabaseAddress(parent);    //  can return nullptr
    return (databaseAddress != nullptr) ?
        WorkspaceAddress(
            _mapDatabaseAddress(databaseAddress),
            [](WorkspaceAddressImpl * p) { delete p; }) :
        WorkspaceAddress();
}

WorkspaceAddress WorkspaceTypeImpl::parseWorkspaceAddress(const QString & externalForm) throws(WorkspaceException)
{
    tt3::db::api::IDatabaseAddress * databaseAddress =
        _databaseType->parseDatabaseAddress(externalForm);  //  never nullptr!
    return (databaseAddress != nullptr) ?
        WorkspaceAddress(
            _mapDatabaseAddress(databaseAddress),
            [](WorkspaceAddressImpl * p) { delete p; }) :
        WorkspaceAddress();
}

//////////
//  Operations (workspace)
Workspace WorkspaceTypeImpl::createWorkspace(
    const WorkspaceAddress & address,
    const QString & adminUser,
    const QString adminLogin, const QString & adminPassword) throws(WorkspaceException)
{
    if (address == nullptr || address->workspaceType() != this)
    {   //  OOPS! Can't use this address
        throw InvalidWorkspaceAddressException();
    }
    std::unique_ptr<tt3::db::api::IDatabase> databasePtr
        { address->_databaseAddress->databaseType()->createDatabase(address->_databaseAddress) };
    //  Create admin user...
    tt3::db::api::IUser * user =
        databasePtr->createUser(
            true,
            QStringList(),
            adminUser,
            std::optional<tt3::util::TimeSpan>(),
            std::optional<QLocale>());
    //  ...and account...
        user->createAccount(
            true,
            QStringList(),
            adminLogin,
            adminPassword,
            tt3::db::api::Capabilities::Administrator);
    //  ...and we're done
    return Workspace(new WorkspaceImpl(address, databasePtr.release()));
    //  TODO translate & re-throw DatabaseException ?,
    //  destroyig the newly created database along the way
}

Workspace WorkspaceTypeImpl::openWorkspace(const WorkspaceAddress & address) throws(WorkspaceException)
{
    if (address == nullptr || address->workspaceType() != this)
    {   //  OOPS! Can't use this address
        throw InvalidWorkspaceAddressException();
    }
    std::unique_ptr<tt3::db::api::IDatabase> databasePtr
        { address->_databaseAddress->databaseType()->openDatabase(address->_databaseAddress) };
    //  TODO translate & re-throw DatabaseException ?
    return Workspace(new WorkspaceImpl(address, databasePtr.release()));
}

/*  TODO uncommnt & implement
void WorkspaceType::destroyWorkspace(const WorkspaceAddress & address) throws(WorkspaceException);
*/

//////////
//  Implementation helpers
WorkspaceAddressImpl * WorkspaceTypeImpl::_mapDatabaseAddress(tt3::db::api::IDatabaseAddress * databaseAddress) const
{
    tt3::util::Lock lock(_addressMapGuard); //  protect the cache!

    if (_addressMap.contains(databaseAddress))
    {   //  Already mapped
        return _addressMap[databaseAddress];
    }
    else
    {   //  Need a new mapping (which auto-registers when constructed)
        return new WorkspaceAddressImpl(const_cast<WorkspaceTypeImpl*>(this), databaseAddress);
    }
}

//  End of tt3-ws/WorkspaceTypeImpl.cpp
