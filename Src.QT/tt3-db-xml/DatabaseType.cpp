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
TT3_IMPLEMENT_SINGLETON(DatabaseType)

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
    static Component::Resources *const resources = Component::Resources::instance();   //  idempotent
    return resources->string(RSID(DatabaseType), RID(DisplayName));
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

QString DatabaseType::shortStatusReport() const
{
    static Component::Resources *const resources = Component::Resources::instance();   //  idempotent
    return resources->string(RSID(DatabaseType), RID(StatusReport));
}

QString DatabaseType::fullStatusReport() const
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
    static Component::Resources *const resources = Component::Resources::instance();   //  idempotent

    QString path =
        QFileDialog::getSaveFileName(
            parent,
            resources->string(RSID(EnterNewDatabaseAddressDialog), RID(Title)),
            /*dir =*/ QString(),
            resources->string(RSID(EnterNewDatabaseAddressDialog), RID(Filter), PreferredExtension));
    if (path.isEmpty())
    {
        return nullptr;
    }
    //  On e.g. Linux we may need to auto-add the extension
    if (QFileInfo(path).suffix().isEmpty())
    {
        path += PreferredExtension;
    }
    //  Go!
    return parseDatabaseAddress(path);
}

auto DatabaseType::enterExistingDatabaseAddress(
        QWidget * parent
    ) -> tt3::db::api::IDatabaseAddress *
{
    static Component::Resources *const resources = Component::Resources::instance();   //  idempotent

    QString path =
        QFileDialog::getOpenFileName(
            parent,
            resources->string(RSID(EnterExistingDatabaseAddressDialog), RID(Title)),
            /*dir =*/ QString(),
            resources->string(RSID(EnterExistingDatabaseAddressDialog), RID(Filter), PreferredExtension));
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
    QString absolutePath = QFileInfo(externalForm).absoluteFilePath();
    if (absolutePath != externalForm)
    {   //  OOPS!
        throw tt3::db::api::InvalidDatabaseAddressException();
    }

    tt3::util::Lock _(_databaseAddressesGuard);

    DatabaseAddress * databaseAddress;
    if (_databaseAddresses.contains(absolutePath))
    {   //  An instance already exists
        databaseAddress = _databaseAddresses[absolutePath];
        if (databaseAddress->_state == DatabaseAddress::State::Old)
        {   //  An instance is Old, but a client just expressed
            //  interest in it, so promote it to New (recount == 0 for both)
            Q_ASSERT(databaseAddress->_referenceCount == 0);
            databaseAddress->_state = DatabaseAddress::State::New;
        }
    }
    else
    {   //  Need a new instance...
        QSet<QString> pathsToRelease;
        for (auto [path, address] : _databaseAddresses.asKeyValueRange())
        {
            if (address->_state == DatabaseAddress::State::Old)
            {   //  Release this one!
                pathsToRelease.insert(path);
            }
        }
        for (const QString & path : pathsToRelease)
        {
            delete _databaseAddresses[path];
            _databaseAddresses.remove(path);
        }
        //  ...so create one
        databaseAddress = new DatabaseAddress(absolutePath);
        Q_ASSERT(databaseAddress->_referenceCount == 0 &&
                 databaseAddress->_state == DatabaseAddress::State::New);
        _databaseAddresses[absolutePath] = databaseAddress;
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
        tt3::db::api::IDatabaseAddress * address,
        tt3::db::api::OpenMode openMode
    ) -> tt3::db::api::IDatabase *
{
    if (DatabaseAddress * xmlDatabaseAddress =
        dynamic_cast<DatabaseAddress*>(address))
    {   //  Address is of a proper type.
        switch (openMode)
        {
            case tt3::db::api::OpenMode::ReadOnly:
                return new Database(xmlDatabaseAddress,
                                    Database::_OpenMode::_OpenReadOnly);
            case tt3::db::api::OpenMode::ReadWrite:
                return new Database(xmlDatabaseAddress,
                                    Database::_OpenMode::_OpenReadWrite);
            case tt3::db::api::OpenMode::Default:
            default:    //  be defensive!
                QFileInfo fileInfo(xmlDatabaseAddress->_path);
                if (fileInfo.isFile() && !fileInfo.isWritable())
                {
                    return new Database(xmlDatabaseAddress,
                                        Database::_OpenMode::_OpenReadOnly);
                }
                //  Fall back to read/write
                return new Database(xmlDatabaseAddress,
                                    Database::_OpenMode::_OpenReadWrite);
        }
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
            { new Database(xmlDatabaseAddress, Database::_OpenMode::_OpenReadWrite) };
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
