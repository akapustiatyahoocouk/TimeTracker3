//
//  tt3-db-api/Exceptions.cpp - DB API exceptions
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

//////////
//  DatabaseException
DatabaseException::DatabaseException()
{
}

//////////
//  InvalidDatabaseAddressException
InvalidDatabaseAddressException::InvalidDatabaseAddressException()
{
}

QString InvalidDatabaseAddressException::errorMessage() const
{
    static Component::Resources *const resources = Component::Resources::instance();   //  idempotent
    return resources->string(
                RSID(Errors),
                RID(InvalidDatabaseAddressException));
}

//////////
//  DatabaseInUseException
DatabaseInUseException::DatabaseInUseException(
        IDatabaseAddress * databaseAddress
    ) : _databaseTypeDisplayName(databaseAddress->databaseType()->displayName()),
        _databaseAddressDisplayForm(databaseAddress->displayForm())
{
}

QString DatabaseInUseException::errorMessage() const
{
    static Component::Resources *const resources = Component::Resources::instance();   //  idempotent
    return resources->string(
                RSID(Errors),
                RID(DatabaseInUseException),
                _databaseTypeDisplayName,
                _databaseAddressDisplayForm);
}

//////////
//  DatabaseCorruptException
DatabaseCorruptException::DatabaseCorruptException(
        IDatabaseAddress * databaseAddress)
    :   _databaseTypeDisplayName(databaseAddress->databaseType()->displayName()),
        _databaseAddressDisplayForm(databaseAddress->displayForm())
{
#ifdef Q_DEBUG
    #if defined(Q_OS_WINDOWS)
        __debugbreak();
    #elif defined(Q_OS_LINUX) || defined(Q_OS_UNIX)
        raise(SIGTRAP)
    #else
        #error Unsupported host OS
    #endif
#endif
}

QString DatabaseCorruptException::errorMessage() const
{
    static Component::Resources *const resources = Component::Resources::instance();   //  idempotent
    return resources->string(
                RSID(Errors),
                RID(DatabaseCorruptException),
                _databaseTypeDisplayName,
                _databaseAddressDisplayForm);
}

//////////
//  DatabaseClosedException
DatabaseClosedException::DatabaseClosedException()
{
}

QString DatabaseClosedException::errorMessage() const
{
    static Component::Resources *const resources = Component::Resources::instance();   //  idempotent
    return resources->string(
                RSID(Errors),
                RID(DatabaseClosedException));
}

//////////
//  AccessDeniedException
AccessDeniedException::AccessDeniedException()
{
}

QString AccessDeniedException::errorMessage() const
{
    static Component::Resources *const resources = Component::Resources::instance();   //  idempotent
    return resources->string(
                RSID(Errors),
                RID(AccessDeniedException));
}

//////////
//  InvalidPropertyValueException
InvalidPropertyValueException::InvalidPropertyValueException(
        const QString & objectTypeName,
        const QString & propertyName,
        const QString & propertyValueString
    ) : _objectTypeName(objectTypeName),
        _propertyName(propertyName),
        _propertyValueString(propertyValueString)
{
}

QString InvalidPropertyValueException::errorMessage() const
{
    static Component::Resources *const resources = Component::Resources::instance();   //  idempotent
    return resources->string(
                RSID(Errors),
                RID(InvalidPropertyValueException),
                _objectTypeName,
                _propertyName,
                _propertyValueString);
}

//////////
//  AlreadyExistsException
AlreadyExistsException::AlreadyExistsException(
        const QString & objectTypeName,
        const QString & propertyName,
        const QString & propertyValueString
    ) : _objectTypeName(objectTypeName),
        _propertyName(propertyName),
        _propertyValueString(propertyValueString)
{
}

QString AlreadyExistsException::errorMessage() const
{
    static Component::Resources *const resources = Component::Resources::instance();   //  idempotent
    return resources->string(
                RSID(Errors),
                RID(AlreadyExistsException),
                _objectTypeName,
                _propertyName,
                _propertyValueString);
}

//////////
//  DoesNotExistException
DoesNotExistException::DoesNotExistException(
        const QString & objectTypeName,
        const QString & propertyName,
        const QString & propertyValueString
    ) : _objectTypeName(objectTypeName),
        _propertyName(propertyName),
        _propertyValueString(propertyValueString)
{
}

QString DoesNotExistException::errorMessage() const
{
    static Component::Resources *const resources = Component::Resources::instance();   //  idempotent
    return resources->string(
                RSID(Errors),
                RID(DoesNotExistException),
                _objectTypeName,
                _propertyName,
                _propertyValueString);
}

//////////
//  InstanceDeadException
InstanceDeadException::InstanceDeadException()
{
}

QString InstanceDeadException::errorMessage() const
{
    static Component::Resources *const resources = Component::Resources::instance();   //  idempotent
    return resources->string(
                RSID(Errors),
                RID(InstanceDeadException));
}

//////////
//  IncompatibleInstanceException
IncompatibleInstanceException::IncompatibleInstanceException(
        const QString & objectTypeName
    ) : _objectTypeName(objectTypeName)
{
}

QString IncompatibleInstanceException::errorMessage() const
{
    static Component::Resources *const resources = Component::Resources::instance();   //  idempotent
    return resources->string(
                RSID(Errors),
                RID(IncompatibleInstanceException),
                _objectTypeName);
}

//////////
//  CustomDatabaseException
CustomDatabaseException::CustomDatabaseException(
        const QString & errorMessage
    ) : _errorMessage(errorMessage)
{
}

QString CustomDatabaseException::errorMessage() const
{
    return _errorMessage;
}

//  End of tt3-db-api/Exceptions.cpp
