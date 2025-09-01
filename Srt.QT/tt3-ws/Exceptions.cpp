//
//  tt3-ws/Exceptions.cpp - Workspace exceptions
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
//  WorkspaceException
WorkspaceException::WorkspaceException()
{
}

Q_NORETURN
void WorkspaceException::translateAndThrow(const tt3::util::Exception & ex) throws(WorkspaceException)
{
    //  Throw workspace exceptions "as is"
    if (auto exx = dynamic_cast<const InvalidWorkspaceAddressException*>(&ex))
    {
        throw *exx;
    }
    else if (auto exx = dynamic_cast<const WorkspaceInUseException*>(&ex))
    {
        throw *exx;
    }
    else if (auto exx = dynamic_cast<const WorkspaceCorruptException*>(&ex))
    {
        throw *exx;
    }
    else if (auto exx = dynamic_cast<const WorkspaceClosedException*>(&ex))
    {
        throw *exx;
    }
    else if (auto exx = dynamic_cast<const AccessDeniedException*>(&ex))
    {
        throw *exx;
    }
    else if (auto exx = dynamic_cast<const InvalidPropertyValueException*>(&ex))
    {
        throw *exx;
    }
    else if (auto exx = dynamic_cast<const AlreadyExistsException*>(&ex))
    {
        throw *exx;
    }
    else if (auto exx = dynamic_cast<const DoesNotExistException*>(&ex))
    {
        throw *exx;
    }
    else if (auto exx = dynamic_cast<const InstanceDeadException*>(&ex))
    {
        throw *exx;
    }
    //  Translate database exeptions to workspace exceptions
    else if (auto exx = dynamic_cast<const tt3::db::api::InvalidDatabaseAddressException*>(&ex))
    {
        throw InvalidWorkspaceAddressException();
    }
    else if (auto exx = dynamic_cast<const tt3::db::api::DatabaseInUseException*>(&ex))
    {
        throw WorkspaceInUseException(
            exx->databaseTypeDisplayName(),
            exx->databaseAddressDisplayForm());
    }
    else if (auto exx = dynamic_cast<const tt3::db::api::DatabaseCorruptException*>(&ex))
    {
        throw WorkspaceCorruptException(
            exx->databaseTypeDisplayName(),
            exx->databaseAddressDisplayForm());
    }
    else if (auto exx = dynamic_cast<const tt3::db::api::DatabaseClosedException*>(&ex))
    {
        throw WorkspaceClosedException();
    }
    else if (auto exx = dynamic_cast<const tt3::db::api::AccessDeniedException*>(&ex))
    {
        throw AccessDeniedException();
    }
    else if (auto exx = dynamic_cast<const tt3::db::api::InvalidPropertyValueException*>(&ex))
    {
        throw InvalidPropertyValueException(
            exx->objectTypeName(),
            exx->propertyName(),
            exx->propertyValueString());
    }
    else if (auto exx = dynamic_cast<const tt3::db::api::AlreadyExistsException*>(&ex))
    {
        throw AlreadyExistsException(
            exx->objectTypeName(),
            exx->propertyName(),
            exx->propertyValueString());
    }
    else if (auto exx = dynamic_cast<const tt3::db::api::DoesNotExistException*>(&ex))
    {
        throw DoesNotExistException(
            exx->objectTypeName(),
            exx->propertyName(),
            exx->propertyValueString());
    }
    else if (auto exx = dynamic_cast<const tt3::db::api::InstanceDeadException*>(&ex))
    {
        throw InstanceDeadException();
    }
    //  Anything else becomes CustomWorkspaceException
    else
    {
        throw CustomWorkspaceException(ex.errorMessage());
    }
}

//////////
//  InvalidWorkspaceAddressException
InvalidWorkspaceAddressException::InvalidWorkspaceAddressException()
{
}

QString InvalidWorkspaceAddressException::errorMessage() const
{
    return "Invalid workspace address";
}

//////////
//  WorkspaceInUseException
WorkspaceInUseException::WorkspaceInUseException(
    const QString & workspaceTypeDisplayName,
    const QString & workspaceAddressDisplayForm)
    :   _workspaceTypeDisplayName(workspaceTypeDisplayName),
        _workspaceAddressDisplayForm(workspaceAddressDisplayForm)
{
}

WorkspaceInUseException::WorkspaceInUseException(const WorkspaceAddress & workspaceAddress)
    :   _workspaceTypeDisplayName(workspaceAddress->workspaceType()->displayName()),
        _workspaceAddressDisplayForm(workspaceAddress->displayForm())
{
}

QString WorkspaceInUseException::errorMessage() const
{
    return _workspaceTypeDisplayName +
           " "  +
           _workspaceAddressDisplayForm +
           " is already in use";
}

//////////
//  WorkspaceCorruptException
WorkspaceCorruptException::WorkspaceCorruptException(
    const QString & workspaceTypeDisplayName,
    const QString & workspaceAddressDisplayForm)
    :   _workspaceTypeDisplayName(workspaceTypeDisplayName),
        _workspaceAddressDisplayForm(workspaceAddressDisplayForm)
{
}

WorkspaceCorruptException::WorkspaceCorruptException(const WorkspaceAddress & workspaceAddress)
    :   _workspaceTypeDisplayName(workspaceAddress->workspaceType()->displayName()),
        _workspaceAddressDisplayForm(workspaceAddress->displayForm())
{
}

QString WorkspaceCorruptException::errorMessage() const
{
    return _workspaceTypeDisplayName +
           " "  +
           _workspaceAddressDisplayForm +
           " is invalid or corrupt";
}

//////////
//  WorkspaceClosedException
WorkspaceClosedException::WorkspaceClosedException()
{
}

QString WorkspaceClosedException::errorMessage() const
{
    return "Workspace is closed";
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
//  AccessWouldBeLostException
AccessWouldBeLostException::AccessWouldBeLostException()
{
}

QString AccessWouldBeLostException::errorMessage() const
{
    return "Access would be lost";
}

//////////
//  CustomWorkspaceException
CustomWorkspaceException::CustomWorkspaceException(const QString & errorMessage)
        :   _errorMessage(errorMessage)
{
}

QString CustomWorkspaceException::errorMessage() const
{
    return _errorMessage;
}

//  End of tt3-ws/Exceptions.cpp
