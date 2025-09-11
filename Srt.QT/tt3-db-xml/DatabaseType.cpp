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
    :   _validator(tt3::db::api::DefaultValidator::instance()),
        //  "Dead" database
        _deadDatabaseAddress(new DatabaseAddress("?")),
        _deadDatabase(new Database(_deadDatabaseAddress, Database::_OpenMode::_Dead))
{
    _databaseAddresses.insert(
        _deadDatabaseAddress->_path,
        _deadDatabaseAddress);
}

DatabaseType::~DatabaseType()
{
}

//////////
//  tt3::db::api::IDatabaseType (general)
tt3::util::Mnemonic DatabaseType::mnemonic() const
{
    return M(XmlFile);
}

QString DatabaseType::displayName() const
{
    return "XML file";
}

QIcon DatabaseType::smallIcon() const
{
    static const QIcon icon(":/tt3-db-xml/Resources/Images/Objects/XmlDatabaseTypeSmall.png");
    return icon;
}

QIcon DatabaseType::largeIcon() const
{
    static const QIcon icon(":/tt3-db-xml/Resources/Images/Objects/XmlDatabaseTypeLarge.png");
    return icon;
}

bool DatabaseType::isOperational() const
{
    return true;
}

auto DatabaseType::shortStatusReport(
    ) const -> QString
{
    return "The XML file storage is operational";
}

auto DatabaseType::fullStatusReport(
    ) const -> QString
{
    return shortStatusReport();
}

auto DatabaseType::validator(
    ) const -> tt3::db::api::IValidator *
{
    return _validator;
}

//////////
//  tt3::db::api::IDatabaseType (address handling)
auto DatabaseType::defaultDatabaseAddress(
    ) const -> tt3::db::api::IDatabaseAddress *
{
    return nullptr;
}

auto DatabaseType::enterNewDatabaseAddress(
        QWidget * parent
    ) -> tt3::db::api::IDatabaseAddress *
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

auto DatabaseType::enterExistingDatabaseAddress(
        QWidget * parent
    ) -> tt3::db::api::IDatabaseAddress *
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

auto DatabaseType::parseDatabaseAddress(
        const QString & externalForm
    ) -> tt3::db::api::IDatabaseAddress *
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
auto DatabaseType::createDatabase(
        tt3::db::api::IDatabaseAddress * address
    ) -> tt3::db::api::IDatabase *
{
    if (DatabaseAddress * xmlDatabaseAddress =
        dynamic_cast<DatabaseAddress*>(address))
    {   //  Address is of a proper type
        return new Database(xmlDatabaseAddress, Database::_OpenMode::_Create);
    }
    throw tt3::db::api::InvalidDatabaseAddressException();
}

auto DatabaseType::openDatabase(
        tt3::db::api::IDatabaseAddress * address
    ) -> tt3::db::api::IDatabase *
{
    if (DatabaseAddress * xmlDatabaseAddress =
        dynamic_cast<DatabaseAddress*>(address))
    {   //  Address is of a proper type.
        return new Database(xmlDatabaseAddress, Database::_OpenMode::_Open);
    }
    throw tt3::db::api::InvalidDatabaseAddressException();
}

void DatabaseType::destroyDatabase(
        tt3::db::api::IDatabaseAddress * address
    )
{
    if (DatabaseAddress * xmlDatabaseAddress =
        dynamic_cast<DatabaseAddress*>(address))
    {   //  Address is of a proper type
        //  Must validate the database file existence and its
        //  contents - the best way is to open it for a moment
        std::unique_ptr<Database> database
            { new Database(xmlDatabaseAddress, Database::_OpenMode::_Open) };
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
