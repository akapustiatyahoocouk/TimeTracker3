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

DatabaseType::DatabaseType()
    :   _smallIcon(":/tt3-db-xml/Resources/Images/Objects/XmlDatabaseTypeSmall.png"),
        _largeIcon(":/tt3-db-xml/Resources/Images/Objects/XmlDatabaseTypeLarge.png"),
        _validator(tt3::db::api::DefaultValidator::instance())
{
}

DatabaseType::~DatabaseType()
{
}

//////////
//  tt3::db::api::IDatabaseType (general)
DatabaseType::Mnemonic DatabaseType::mnemonic() const
{
    return M(XmlFile);
}

QString DatabaseType::displayName() const
{
    return "XML file";
}

QIcon DatabaseType::smallIcon() const
{
    return _smallIcon;
}

QIcon DatabaseType::largeIcon() const
{
    return _largeIcon;
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

tt3::db::api::IValidator * DatabaseType::validator() const
{
    return _validator;
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
            "XML database files (*.tt3);;All files (*.*)");
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
            "XML database files (*.tt3);;All files (*.*)");
    if (path.isEmpty())
    {
        return nullptr;
    }
    return parseDatabaseAddress(path);
}

tt3::db::api::IDatabaseAddress * DatabaseType::parseDatabaseAddress(const QString & externalForm) throws(tt3::db::api::DatabaseException)
{
    QString path = QFileInfo(externalForm).absoluteFilePath();
    if (path != externalForm)
    {   //  OOPS!
        throw tt3::db::api::InvalidDatabaseAddressException();
    }

    tt3::util::Lock lock(_databaseAddressesGuard);

    DatabaseAddress * databaseAddress;
    if (_databaseAddresses.contains(path))
    {   //  An instance already exists
        databaseAddress = _databaseAddresses[path];
        if (databaseAddress->_state == DatabaseAddress::State::Old)
        {   //  An instance is Old, but a client just
            //  expressed interest in it, so promote
            //  it to New
            databaseAddress->_state = DatabaseAddress::State::New;
        }
    }
    else
    {   //  Need a new instance
        databaseAddress = new DatabaseAddress(path);
        Q_ASSERT(databaseAddress->_referenceCount == 0 &&
                 databaseAddress->_state == DatabaseAddress::State::New);
        _databaseAddresses[path] = databaseAddress;
    }
#ifdef QT_DEBUG
    databaseAddress->_assertState();
#endif
    return databaseAddress;
}

//////////
//  tt3::db::api::IDatabaseType (databases)
tt3::db::api::IDatabase * DatabaseType::createDatabase(tt3::db::api::IDatabaseAddress * address) throws(tt3::db::api::DatabaseException)
{
    if (DatabaseAddress * xmlDatabaseAddress =
        dynamic_cast<DatabaseAddress*>(address))
    {   //  Address is of a proper type
        return new Database(xmlDatabaseAddress, Database::_Mode::_Create);
    }
    throw tt3::db::api::InvalidDatabaseAddressException();
}

tt3::db::api::IDatabase * DatabaseType::openDatabase(tt3::db::api::IDatabaseAddress * address) throws(tt3::db::api::DatabaseException)
{
    if (DatabaseAddress * xmlDatabaseAddress =
        dynamic_cast<DatabaseAddress*>(address))
    {   //  Address is of a proper type.
        return new Database(xmlDatabaseAddress, Database::_Mode::_Open);
    }
    throw tt3::db::api::InvalidDatabaseAddressException();
}

void DatabaseType::destroyDatabase(tt3::db::api::IDatabaseAddress * address) throws(tt3::db::api::DatabaseException)
{
    if (DatabaseAddress * xmlDatabaseAddress =
        dynamic_cast<DatabaseAddress*>(address))
    {   //  Address is of a proper type
        //  Must validate the database file existence and its
        //  contents - the best way is to open it for a moment
        std::unique_ptr<Database> database
            { new Database(xmlDatabaseAddress, Database::_Mode::_Open) };
        database->close();
        QFile file(xmlDatabaseAddress->_path);
        if (!file.remove())
        {   //  OOPS!
            throw tt3::db::api::CustomDatabaseException(file.errorString());
        }
        return;
    }
    throw tt3::db::api::InvalidDatabaseAddressException();
}

//  End of tt3-db-xml/DatabaseType.cpp
