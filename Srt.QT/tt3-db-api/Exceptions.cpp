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
    return "Invalid database address";
}

//////////
//  DatabaseInUseException
DatabaseInUseException::DatabaseInUseException(
    IDatabaseAddress * databaseAddress)
    :   _databaseTypeDisplayName(databaseAddress->databaseType()->displayName()),
        _databaseAddressDisplayForm(databaseAddress->displayForm())
{
}

QString DatabaseInUseException::errorMessage() const
{
    return _databaseTypeDisplayName +
           " "  +
           _databaseAddressDisplayForm +
           " is already in use";
}

//////////
//  DatabaseCorruptException
DatabaseCorruptException::DatabaseCorruptException(
    IDatabaseAddress * databaseAddress)
    :   _databaseTypeDisplayName(databaseAddress->databaseType()->displayName()),
        _databaseAddressDisplayForm(databaseAddress->displayForm())
{
}

QString DatabaseCorruptException::errorMessage() const
{
    return _databaseTypeDisplayName +
           " "  +
           _databaseAddressDisplayForm +
           " is invalid or corrupt";
}

//////////
//  DatabaseClosedException
DatabaseClosedException::DatabaseClosedException()
{
}

QString DatabaseClosedException::errorMessage() const
{
    return "Daabase is closed";
}

//////////
//  AccessDeniedException
AccessDeniedException::AccessDeniedException()
{
}

QString AccessDeniedException::errorMessage() const
{
    return "Access denied";
}

//////////
//  InvalidPropertyValueException
InvalidPropertyValueException::InvalidPropertyValueException(
    const QString & objectTypeName,
    const QString & propertyName, const QString & propertyValueString)
    :   _objectTypeName(objectTypeName),
        _propertyName(propertyName),
        _propertyValueString(propertyValueString)
{
}

QString InvalidPropertyValueException::errorMessage() const
{
    return "Property '" + _propertyName +
           "' of '" + _objectTypeName +
           "' cannot be set to '" + _propertyValueString + "'";
}

//////////
//  AlreadyExistsException
AlreadyExistsException::AlreadyExistsException(
    const QString & objectTypeName,
    const QString & propertyName, const QString & propertyValueString)
    :   _objectTypeName(objectTypeName),
        _propertyName(propertyName),
        _propertyValueString(propertyValueString)
{
}

QString AlreadyExistsException::errorMessage() const
{
    return "The " + _objectTypeName +
           " with '" + _propertyName +
           "' = '" + _propertyValueString +
           "' already exists";
}

//////////
//  DoesNotExistException
DoesNotExistException::DoesNotExistException(
    const QString & objectTypeName,
    const QString & propertyName, const QString & propertyValueString)
    :   _objectTypeName(objectTypeName),
        _propertyName(propertyName),
        _propertyValueString(propertyValueString)
{
}

QString DoesNotExistException::errorMessage() const
{
    return "The " + _objectTypeName +
           " with '" + _propertyName +
           "' = '" + _propertyValueString +
           "' does not exist";
}

//////////
//  InstanceDeadException
InstanceDeadException::InstanceDeadException()
{
}

QString InstanceDeadException::errorMessage() const
{
    return "The instance is dead";
}

//////////
//  CustomDatabaseException
CustomDatabaseException::CustomDatabaseException(const QString & errorMessage)
    :   _errorMessage(errorMessage)
{
}

QString CustomDatabaseException::errorMessage() const
{
    return _errorMessage;
}


//  End of tt3-db-api/Exceptions.cpp
