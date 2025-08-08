//
//  tt3-db-xml/DatabaseType.cpp - tt3::db::xml::DatabaseType class implementation
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
#include "tt3-db-xml/API.hpp"
using namespace tt3::db::xml;

//////////
//  Sigleton
IMPLEMENT_SINGLETON(DatabaseType)
DatabaseType::DatabaseType() {}
DatabaseType::~DatabaseType() {}

//////////
//  tt3::db::api::IDatabaseType (general)
QString DatabaseType::mnemonic() const
{
    return "XmlFile";
}

QString DatabaseType::displayName() const
{
    return "XML file";
}

bool DatabaseType::isOperational() const
{
    return true;
}

QString DatabaseType::shortStatusReport() const
{
    return "The XML file storage is operational";
}

QString DatabaseType::fullStatusReport() const
{
    return shortStatusReport();
}

//////////
//  tt3::db::api::IDatabaseType (address handling)
tt3::db::api::IDatabaseAddress * DatabaseType::defaultDatabaseAddress() const
{
    return nullptr;
}

tt3::db::api::IDatabaseAddress * DatabaseType::enterNewDatabaseAddress(QWidget * parent)
{
    QString path =
        QFileDialog::getSaveFileName(
            parent,
            "Create XML file database",
            /*dir =*/ QString(),
            "XML database files (*.tt3);;Text files (*.txt)");
    if (path.isEmpty())
    {
        return nullptr;
    }
    return parseDatabaseAddress(path);
}

tt3::db::api::IDatabaseAddress * DatabaseType::enterExistingDatabaseAddress(QWidget * parent)
{
    QString path =
        QFileDialog::getOpenFileName(
            parent,
            "Select XML file database",
            /*dir =*/ QString(),
            "XML database files (*.tt3);;Text files (*.txt)");
    if (path.isEmpty())
    {
        return nullptr;
    }
    return parseDatabaseAddress(path);
}

tt3::db::api::IDatabaseAddress * DatabaseType::parseDatabaseAddress(const QString & externalForm) throws(DatabaseException)
{
    QString path = QFileInfo(externalForm).absoluteFilePath();
    if (path != externalForm)
    {   //  OOPS!
        throw tt3::db::api::InvalidDatabaseAddressException();
    }

    QMutexLocker<QMutex> locker(&_databaseAddressesGuard);

    if (_newDatabaseAddresses.contains(path))
    {
        return _newDatabaseAddresses[path];
    }
    if (_managedDatabaseAddresses.contains(path))
    {
        return _managedDatabaseAddresses[path];
    }
    //  Don't examine _oldDatabaseAddresses - these can
    //  be recycled at any moment.
    //  So, need a new Address instance.
    //  TODO what if a reference is added to an Old address?
    DatabaseAddress * databaseAddress = new DatabaseAddress(path);
    Q_ASSERT(databaseAddress->_referenceCount == 0 &&
             databaseAddress->_state == DatabaseAddress::State::New);
    _newDatabaseAddresses[path] = databaseAddress;
    return databaseAddress;
}

//////////
//  tt3::db::api::IDatabaseType (databases)
tt3::db::api::IDatabase * DatabaseType::createDatabase(tt3::db::api::IDatabaseAddress * address) throws(DatabaseException)
{
    if (DatabaseAddress * xmlDatabaseAddress =
        dynamic_cast<DatabaseAddress*>(address))
    {   //  Address is of a proper type
        throw tt3::db::api::DatabaseException("Not yet implemented");
    }
    throw tt3::db::api::InvalidDatabaseAddressException();
}

tt3::db::api::IDatabase * DatabaseType::openDatabase(tt3::db::api::IDatabaseAddress * address) throws(DatabaseException)
{
    if (DatabaseAddress * xmlDatabaseAddress =
        dynamic_cast<DatabaseAddress*>(address))
    {   //  Address is of a proper type
        throw tt3::db::api::DatabaseException("Not yet implemented");
    }
    throw tt3::db::api::InvalidDatabaseAddressException();
}

void DatabaseType::destroyDatabase(tt3::db::api::IDatabaseAddress * address) throws(DatabaseException)
{
    if (DatabaseAddress * xmlDatabaseAddress =
        dynamic_cast<DatabaseAddress*>(address))
    {   //  Address is of a proper type
        throw tt3::db::api::DatabaseException("Not yet implemented");
    }
    throw tt3::db::api::InvalidDatabaseAddressException();
}

//  End of tt3-db-xml/DatabaseType.cpp
