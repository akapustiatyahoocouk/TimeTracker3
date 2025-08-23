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

//  TODO implement
//  Q_NORETURN
//  void WorkspaceException::translateAndThrow(const tt3::util::Exception & ex) throws(WorkspaceException);

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
