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
    _addressMap.clear();
}

//////////
//  Operation (general)
WorkspaceTypeImpl::Mnemonic WorkspaceTypeImpl::mnemonic() const
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
            const_cast<WorkspaceTypeImpl*>(this)->_mapDatabaseAddress(databaseAddress) :
            nullptr;
}

WorkspaceAddress WorkspaceTypeImpl::enterNewWorkspaceAddress(QWidget * parent)
{
    tt3::db::api::IDatabaseAddress * databaseAddress =
        _databaseType->enterNewDatabaseAddress(parent); //  can return nullptr
    return (databaseAddress != nullptr) ?
            _mapDatabaseAddress(databaseAddress) :
            nullptr;
}

WorkspaceAddress WorkspaceTypeImpl::enterExistingWorkspaceAddress(QWidget * parent)
{
    tt3::db::api::IDatabaseAddress * databaseAddress =
        _databaseType->enterExistingDatabaseAddress(parent);    //  can return nullptr
    return (databaseAddress != nullptr) ?
            _mapDatabaseAddress(databaseAddress) :
            nullptr;
}

WorkspaceAddress WorkspaceTypeImpl::parseWorkspaceAddress(const QString & externalForm) throws(WorkspaceException)
{
    tt3::db::api::IDatabaseAddress * databaseAddress =
        _databaseType->parseDatabaseAddress(externalForm);  //  never nullptr!
    return (databaseAddress != nullptr) ?
            _mapDatabaseAddress(databaseAddress) :
            nullptr;
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
    try
    {
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
        /*  NOTE that database change notifications caused
            by creating admin user and account (above) are
            NOT forwarded to the Workspace, as the Workspace
            instance has NOT yet been created.  */
        //  ...and we're done.
        return Workspace(
                new WorkspaceImpl(address, databasePtr.release()),
                [](WorkspaceImpl * p) { delete p; });
    }
    catch (const tt3::util::Exception & ex)
    {   //  OOPS! Translate & re-throw, but first...
        try
        {   //  ..destroy the newly created database!
            address->_databaseAddress->databaseType()->destroyDatabase(address->_databaseAddress);
        }
        catch (const tt3::util::Exception &)
        {   //  OOPS! TODO log?
        }
        WorkspaceException::translateAndThrow(ex);
    }
}

Workspace WorkspaceTypeImpl::openWorkspace(const WorkspaceAddress & address) throws(WorkspaceException)
{
    if (address == nullptr || address->workspaceType() != this)
    {   //  OOPS! Can't use this address
        throw InvalidWorkspaceAddressException();
    }
    try
    {
        std::unique_ptr<tt3::db::api::IDatabase> databasePtr
            { address->_databaseAddress->databaseType()->openDatabase(address->_databaseAddress) };
        return Workspace(
            new WorkspaceImpl(address, databasePtr.release()),
            [](WorkspaceImpl * p) { delete p; });
    }
    catch (const tt3::util::Exception & ex)
    {   //  OOPS! Translate & re-throw
        WorkspaceException::translateAndThrow(ex);
    }
}

/*  TODO uncomment & implement
void WorkspaceType::destroyWorkspace(const WorkspaceAddress & address) throws(WorkspaceException);
*/

//////////
//  Implementation helpers
WorkspaceAddress WorkspaceTypeImpl::_mapDatabaseAddress(tt3::db::api::IDatabaseAddress * databaseAddress)
{
    tt3::util::Lock lock(_addressMapGuard); //  protect the cache!

    if (_addressMap.contains(databaseAddress))
    {   //  Already mapped
        return _addressMap[databaseAddress];
    }
    else
    {   //  Need a new mapping (which auto-registers when constructed).
        //  Note that we're increasing the address cache size, so it is
        //  prudent that we keep it in check
        _prunWorkspaceAddresses();
        WorkspaceAddress workspaceAddress(
            new WorkspaceAddressImpl(this, databaseAddress),
            [](WorkspaceAddressImpl * p) { delete p; });
        _addressMap.insert(databaseAddress, workspaceAddress);
        return workspaceAddress;
    }
}

void WorkspaceTypeImpl::_prunWorkspaceAddresses()
{
    for (tt3::db::api::IDatabaseAddress * key : _addressMap.keys())
    {
        Q_ASSERT(_addressMap[key].use_count() > 0);
        if (_addressMap[key].use_count() == 1)
        {   //  WorkspaceAddress is ONLY referred to by _addressMap
            _addressMap.remove(key);
        }
    }
}

//  End of tt3-ws/WorkspaceTypeImpl.cpp
