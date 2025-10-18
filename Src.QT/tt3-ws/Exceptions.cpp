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
void WorkspaceException::translateAndThrow(
        const tt3::util::Exception & ex
    )
{
    //  Throw workspace exceptions "as is"
    if (auto exx = dynamic_cast<const WorkspaceException*>(&ex))
    {
        exx->raise();
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
    else if (auto exx = dynamic_cast<const tt3::db::api::IncompatibleInstanceException*>(&ex))
    {
        throw IncompatibleInstanceException(exx->objectTypeName());
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
    static Component::Resources *const resources = Component::Resources::instance();   //  idempotent
    return resources->string(
            RSID(Errors),
            RID(InvalidWorkspaceAddressException));
}

//////////
//  WorkspaceInUseException
WorkspaceInUseException::WorkspaceInUseException(
        const QString & workspaceTypeDisplayName,
        const QString & workspaceAddressDisplayForm
    ) : _workspaceTypeDisplayName(workspaceTypeDisplayName),
        _workspaceAddressDisplayForm(workspaceAddressDisplayForm)
{
}

WorkspaceInUseException::WorkspaceInUseException(
        const WorkspaceAddress & workspaceAddress
    ) : _workspaceTypeDisplayName(workspaceAddress->workspaceType()->displayName()),
        _workspaceAddressDisplayForm(workspaceAddress->displayForm())
{
}

QString WorkspaceInUseException::errorMessage() const
{
    static Component::Resources *const resources = Component::Resources::instance();   //  idempotent
    return resources->string(
            RSID(Errors),
            RID(WorkspaceInUseException),
            _workspaceTypeDisplayName,
            _workspaceAddressDisplayForm);
}

//////////
//  WorkspaceCorruptException
WorkspaceCorruptException::WorkspaceCorruptException(
        const QString & workspaceTypeDisplayName,
        const QString & workspaceAddressDisplayForm
    ) : _workspaceTypeDisplayName(workspaceTypeDisplayName),
        _workspaceAddressDisplayForm(workspaceAddressDisplayForm)
{
}

WorkspaceCorruptException::WorkspaceCorruptException(
        const WorkspaceAddress & workspaceAddress
    ) : _workspaceTypeDisplayName(workspaceAddress->workspaceType()->displayName()),
        _workspaceAddressDisplayForm(workspaceAddress->displayForm())
{
}

QString WorkspaceCorruptException::errorMessage() const
{
    static Component::Resources *const resources = Component::Resources::instance();   //  idempotent
    return resources->string(
            RSID(Errors),
            RID(WorkspaceCorruptException),
            _workspaceTypeDisplayName,
            _workspaceAddressDisplayForm);
}

//////////
//  WorkspaceClosedException
WorkspaceClosedException::WorkspaceClosedException()
{
}

QString WorkspaceClosedException::errorMessage() const
{
    static Component::Resources *const resources = Component::Resources::instance();   //  idempotent
    return resources->string(
            RSID(Errors),
            RID(WorkspaceClosedException));
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
//  AccessWouldBeLostException
AccessWouldBeLostException::AccessWouldBeLostException()
{
}

QString AccessWouldBeLostException::errorMessage() const
{
    static Component::Resources *const resources = Component::Resources::instance();   //  idempotent
    return resources->string(
            RSID(Errors),
            RID(AccessWouldBeLostException));
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
//  CustomWorkspaceException
CustomWorkspaceException::CustomWorkspaceException(
        const QString & errorMessage
    )
    :   _errorMessage(errorMessage)
{
}

QString CustomWorkspaceException::errorMessage() const
{
    return _errorMessage;
}

//  End of tt3-ws/Exceptions.cpp
