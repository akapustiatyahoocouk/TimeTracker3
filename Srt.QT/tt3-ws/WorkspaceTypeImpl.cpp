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
    _workspaceAddressCache.clear();
    _workspaceCache.clear();
}

//////////
//  Operation (general)
auto WorkspaceTypeImpl::mnemonic(
    ) const -> tt3::util::Mnemonic
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
auto WorkspaceTypeImpl::defaultWorkspaceAddress(
    ) const -> WorkspaceAddress
{
    tt3::db::api::IDatabaseAddress * databaseAddress =
        _databaseType->defaultDatabaseAddress();    //  can return nullptr
    return (databaseAddress != nullptr) ?
            const_cast<WorkspaceTypeImpl*>(this)->_mapDatabaseAddress(databaseAddress) :
            nullptr;
}

auto WorkspaceTypeImpl::enterNewWorkspaceAddress(
        QWidget * parent
    ) -> WorkspaceAddress
{
    tt3::db::api::IDatabaseAddress * databaseAddress =
        _databaseType->enterNewDatabaseAddress(parent); //  can return nullptr
    return (databaseAddress != nullptr) ?
            _mapDatabaseAddress(databaseAddress) :
            nullptr;
}

auto WorkspaceTypeImpl::enterExistingWorkspaceAddress(
        QWidget * parent
    ) -> WorkspaceAddress
{
    tt3::db::api::IDatabaseAddress * databaseAddress =
        _databaseType->enterExistingDatabaseAddress(parent);    //  can return nullptr
    return (databaseAddress != nullptr) ?
            _mapDatabaseAddress(databaseAddress) :
            nullptr;
}

auto WorkspaceTypeImpl::parseWorkspaceAddress(
        const QString & externalForm
    ) -> WorkspaceAddress
{
    tt3::db::api::IDatabaseAddress * databaseAddress =
        _databaseType->parseDatabaseAddress(externalForm);  //  never nullptr!
    return (databaseAddress != nullptr) ?
            _mapDatabaseAddress(databaseAddress) :
            nullptr;
}

//////////
//  Operations (workspace)
auto WorkspaceTypeImpl::createWorkspace(
        const WorkspaceAddress & address,
        const QString & adminUser,
        const QString & adminLogin,
        const QString & adminPassword
    ) -> Workspace
{
    if (address == nullptr || address->workspaceType() != this)
    {   //  OOPS! Can't use this address
        throw InvalidWorkspaceAddressException();
    }
    try
    {
        std::unique_ptr<tt3::db::api::IDatabase> database
            { address->_databaseAddress->databaseType()->createDatabase(address->_databaseAddress) };
        //  Create admin user...
        tt3::db::api::IUser * user =
            database->createUser(
                true,
                QStringList(),
                adminUser,
                InactivityTimeout(),
                UiLocale());
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
        return _mapWorkspace(new WorkspaceImpl(address, database.release()));
    }
    catch (const tt3::util::Exception & ex)
    {   //  OOPS! Translate & re-throw, but first...
        try
        {   //  ..destroy the newly created database!
            address->_databaseAddress->databaseType()->destroyDatabase(address->_databaseAddress);
        }
        catch (const tt3::util::Exception & ex)
        {   //  OOPS! Log, though
            qCritical() << ex.errorMessage();
        }
        WorkspaceException::translateAndThrow(ex);
    }
}

auto WorkspaceTypeImpl::openWorkspace(
        const WorkspaceAddress & address,
        OpenMode openMode
    ) -> Workspace
{
    if (address == nullptr || address->workspaceType() != this)
    {   //  OOPS! Can't use this address
        throw InvalidWorkspaceAddressException();
    }
    try
    {
        std::unique_ptr<tt3::db::api::IDatabase> database
            { address->_databaseAddress->databaseType()->openDatabase(address->_databaseAddress, openMode) };
        return _mapWorkspace(new WorkspaceImpl(address, database.release()));
    }
    catch (const tt3::util::Exception & ex)
    {   //  OOPS! Translate & re-throw
        WorkspaceException::translateAndThrow(ex);
    }
}

void WorkspaceTypeImpl::destroyWorkspace(
        const Credentials & credentials,
        const WorkspaceAddress & address
    )
{
    if (address == nullptr || address->workspaceType() != this)
    {   //  OOPS! Can't use this address
        throw InvalidWorkspaceAddressException();
    }
    try
    {
        //  We need to open the database to verify
        //  the admin credentials for it
        std::unique_ptr<tt3::db::api::IDatabase> database
            { address->_databaseAddress->databaseType()->openDatabase(address->_databaseAddress, OpenMode::ReadWrite) };
        tt3::db::api::IAccount * account = database->login(credentials._login, credentials._password);
        if ((account->capabilities() & Capabilities::Administrator) == Capabilities::None)
        {   //  OOPS! Not allowed!
            throw AccessDeniedException();
        }
        //  Now we can close the database and destroy it immediately
        database->close();
        address->_databaseAddress->databaseType()->destroyDatabase(address->_databaseAddress);
    }
    catch (const tt3::util::Exception & ex)
    {   //  OOPS! Translate & re-throw
        WorkspaceException::translateAndThrow(ex);
    }
}

//////////
//  Implementation helpers
WorkspaceAddress WorkspaceTypeImpl::_mapDatabaseAddress(tt3::db::api::IDatabaseAddress * databaseAddress)
{
    tt3::util::Lock lock(_cacheGuard);  //  protect the cache!

    if (_workspaceAddressCache.contains(databaseAddress))
    {   //  Already mapped
        return _workspaceAddressCache[databaseAddress];
    }
    else
    {   //  Need a new mapping.
        //  Note that we're increasing the address cache size, so it is
        //  prudent that we keep it in check
        _pruneWorkspaceAddressCache();
        WorkspaceAddress workspaceAddress(
            new WorkspaceAddressImpl(this, databaseAddress),
            [](WorkspaceAddressImpl * p) { delete p; });
        _workspaceAddressCache.insert(databaseAddress, workspaceAddress);
        return workspaceAddress;
    }
}

void WorkspaceTypeImpl::_pruneWorkspaceAddressCache()
{
    tt3::util::Lock lock(_cacheGuard);  //  protect the cache!

    for (tt3::db::api::IDatabaseAddress * key : _workspaceAddressCache.keys())
    {
        Q_ASSERT(_workspaceAddressCache[key].use_count() > 0);
        if (_workspaceAddressCache[key].use_count() == 1)
        {   //  WorkspaceAddress is ONLY referred to by _workspaceAddressCache
            _workspaceAddressCache.remove(key);
        }
    }
}

Workspace WorkspaceTypeImpl::_mapWorkspace(WorkspaceImpl * impl)
{
    tt3::util::Lock lock(_cacheGuard);  //  protect the cache!

    if (_workspaceCache.contains(impl))
    {   //  Already mapped
        return _workspaceCache[impl];
    }
    else
    {   //  Need a new mapping.
        //  Note that we're increasing the workspace cache size,
        //  so it is  prudent that we keep it in check
        _pruneWorkspaceCache();
        Workspace workspace(
            impl,
            [](WorkspaceImpl * p) { delete p; });
        _workspaceCache.insert(impl, workspace);
        return workspace;
    }
}

void WorkspaceTypeImpl::_pruneWorkspaceCache()
{
    tt3::util::Lock lock(_cacheGuard);  //  protect the cache!

    for (WorkspaceImpl * key : _workspaceCache.keys())
    {
        Q_ASSERT(_workspaceCache[key].use_count() > 0);
        if (_workspaceCache[key].use_count() == 1)
        {   //  Workspace is ONLY referred to by _workspaceCache
            _workspaceCache.remove(key);
        }
    }
}

//  End of tt3-ws/WorkspaceTypeImpl.cpp
