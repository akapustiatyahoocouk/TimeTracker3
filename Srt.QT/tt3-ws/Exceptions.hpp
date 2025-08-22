//
//  tt3-ws/Exceptions.hpp - Workspace - level exceptions
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

namespace tt3::ws
{
    //  TODO move implementations to Exceptions.cpp
    //////////
    //  The common base class for all workspace - level exceptions
    class TT3_WS_PUBLIC WorkspaceException : public tt3::util::Exception
    {
        //////////
        //  Construction/destruction/assignment
    public:
        WorkspaceException() = default;

        //////////
        //  Operations
    public:
        //  TODO document and TODO implement
        Q_NORETURN
        static void     translateAndThrow(const tt3::util::Exception & ex) throws(WorkspaceException);
    };

    //  Thrown when an invalid workspace address is specified
    class TT3_WS_PUBLIC InvalidWorkspaceAddressException : public WorkspaceException
    {
        //////////
        //  Types:
        using Self = InvalidWorkspaceAddressException;

        //////////
        //  Construction/destruction/assignment
    public:
        InvalidWorkspaceAddressException() = default;

        //////////
        //  QException
    public:
        virtual Self *  clone() const { return new Self(*this); }
        virtual void    raise() const { throw *this; }

        //////////
        //  tt3::util::Exception
    public:
        virtual QString errorMessage() const override
        {
            return "Invalid workspace address";
        }
    };

    //  Thrown when a single-user workspace is already in use
    class TT3_WS_PUBLIC WorkspaceInUseException : public WorkspaceException
    {
        //////////
        //  Types
    public:
        using Self = WorkspaceInUseException;

        //////////
        //  Construction/destruction/assignment
    public:
        explicit WorkspaceInUseException(const WorkspaceAddress & workspaceAddress)
            :   _workspaceTypeDisplayName(workspaceAddress.workspaceType()->displayName()),
                _workspaceAddressDisplayForm(workspaceAddress.displayForm()) {}

        //////////
        //  QException
    public:
        virtual Self *  clone() const { return new Self(*this); }
        virtual void    raise() const { throw *this; }

        //////////
        //  tt3::util::Exception
    public:
        virtual QString errorMessage() const override
        {
            return _workspaceTypeDisplayName +
                   " "  +
                   _workspaceAddressDisplayForm +
                   " is already in use";
        }

        //////////
        //  Operations
    public:
        //  The user-readable display name of the workspace type
        QString         workspaceTypeDisplayName() const { return _workspaceTypeDisplayName; }

        //  Returns the user-readable display form of the
        //  address of the workspace that is already in use
        QString         workspaceAddressDisplayForm() const { return _workspaceAddressDisplayForm; }

        //////////
        //  Implementayion
    private:
        QString         _workspaceTypeDisplayName;
        QString         _workspaceAddressDisplayForm;
    };

    //  Thrown when a workspace is corrupt
    class TT3_WS_PUBLIC WorkspaceCorruptException : public WorkspaceException
    {
        //////////
        //  Types
    public:
        using Self = WorkspaceCorruptException;

        //////////
        //  Construction/destruction/assignment
    public:
        explicit WorkspaceCorruptException(const WorkspaceAddress & workspaceAddress)
            :   _workspaceTypeDisplayName(workspaceAddress.workspaceType()->displayName()),
                _workspaceAddressDisplayForm(workspaceAddress.displayForm()) {}

        //////////
        //  QException
    public:
        virtual Self *  clone() const { return new Self(*this); }
        virtual void    raise() const { throw *this; }

        //////////
        //  tt3::util::Exception
    public:
        virtual QString errorMessage() const override
        {
            return _workspaceTypeDisplayName +
                   " "  +
                   _workspaceAddressDisplayForm +
                   " is invalid or corrupt";
        }

        //////////
        //  Operations
    public:
        //  The user-readable display name of the workspace type
        QString         workspaceTypeDisplayName() const { return _workspaceTypeDisplayName; }

        //  Returns the user-readable display form of the
        //  address of the workspace that is already in use
        QString         workspaceAddressDisplayForm() const { return _workspaceAddressDisplayForm; }

        //////////
        //  Implementayion
    private:
        QString     _workspaceTypeDisplayName;
        QString     _workspaceAddressDisplayForm;
    };

    //  Thrown when a service is called on a closed Workspace.
    class TT3_WS_PUBLIC WorkspaceClosedException : public WorkspaceException
    {
        //////////
        //  Types
    public:
        using Self = WorkspaceClosedException;

        //////////
        //  Construction/destruction/assignment
    public:
        WorkspaceClosedException() = default;

        //////////
        //  QException
    public:
        virtual Self *  clone() const { return new Self(*this); }
        virtual void    raise() const { throw *this; }

        //////////
        //  tt3::util::Exception
    public:
        virtual QString errorMessage() const override
        {
            return "Daabase is closed";
        }
    };

    //  Thrown when a workspace service is access-denied
    class TT3_WS_PUBLIC AccessDeniedException : public WorkspaceException
    {
        //////////
        //  Types
    public:
        using Self = AccessDeniedException;

        //////////
        //  Construction/destruction/assignment
    public:
        AccessDeniedException() = default;

        //////////
        //  QException
    public:
        virtual Self *  clone() const { return new Self(*this); }
        virtual void    raise() const { throw *this; }

        //////////
        //  tt3::util::Exception
    public:
        virtual QString errorMessage() const override
        {
            return "Access denied";
        }
    };

    //  Thrown when attempting to assign an invalid value to a property
    class TT3_WS_PUBLIC InvalidPropertyValueException : public WorkspaceException
    {
        //////////
        //  Types
    public:
        using Self = InvalidPropertyValueException;

        //////////
        //  Construction/destruction/assignment
    public:
        InvalidPropertyValueException(const QString & objectTypeName,
                                      const QString & propertyName, const QString & propertyValueString)
            :   _objectTypeName(objectTypeName),
                _propertyName(propertyName),
                _propertyValueString(propertyValueString) {}
        InvalidPropertyValueException(ObjectType * objectType,
                                      const QString & propertyName, const QString & propertyValueString)
            :   Self(objectType->displayName(), propertyName, propertyValueString) {}

        template <class T>
        InvalidPropertyValueException(ObjectType * objectType,
                                      const QString & propertyName, const T & propertyValue)
            :   Self(objectType->displayName(), propertyName, tt3::util::toString(propertyValue)) {}
        template <class T>
        InvalidPropertyValueException(const QString & objectTypeName,
                                      const QString & propertyName, const T & propertyValue)
            :   Self(objectTypeName, propertyName, tt3::util::toString(propertyValue)) {}

        //////////
        //  QException
    public:
        virtual Self *  clone() const { return new Self(*this); }
        virtual void    raise() const { throw *this; }

        //////////
        //  tt3::util::Exception
    public:
        virtual QString errorMessage() const override
        {
            return "Property '" + _propertyName +
                   "' of '" + _objectTypeName +
                   "' cannot be set to '" + _propertyValueString + "'";
        }

        //////////
        //  Operations
    public:
        //  Thw ibternal name of the object type.
        QString         objectTypeName() const { return _objectTypeName; }

        //  The name of the property.
        QString         propertyName() const { return _propertyName; }

        //  The string representation of the offending property value.
        QString         propertyValueString() const { return _propertyValueString; }

        //////////
        //  Implementation
    private:
        QString         _objectTypeName;
        QString         _propertyName;
        QString         _propertyValueString;
    };

    //  Thrown when an object with the specified property value,
    //  which is supposed to be unique, already exists
    class TT3_WS_PUBLIC AlreadyExistsException : public WorkspaceException
    {
        //////////
        //  Types
    public:
        using Self = AlreadyExistsException;

        //////////
        //  Construction/destruction/assignment
    public:
        AlreadyExistsException(const QString & objectTypeName,
                               const QString & propertyName, const QString & propertyValueString)
            :   _objectTypeName(objectTypeName),
                _propertyName(propertyName),
                _propertyValueString(propertyValueString) {}
        AlreadyExistsException(ObjectType * objectType,
                               const QString & propertyName, const QString & propertyValueString)
            :   Self(objectType->displayName(), propertyName, propertyValueString) {}

        template <class T>
        AlreadyExistsException(ObjectType * objectType,
                               const QString & propertyName, const T & propertyValue)
            :   Self(objectType->displayName(), propertyName, tt3::util::toString(propertyValue)) {}
        template <class T>
        AlreadyExistsException(const QString & objectTypeName,
                               const QString & propertyName, const T & propertyValue)
            :   Self(objectTypeName, propertyName, tt3::util::toString(propertyValue)) {}

        //////////
        //  QException
    public:
        virtual Self *  clone() const { return new Self(*this); }
        virtual void    raise() const { throw *this; }

        //////////
        //  tt3::util::Exception
    public:
        virtual QString errorMessage() const override
        {
            return "The " + _objectTypeName +
                   " with '" + _propertyName +
                   "' = '" + _propertyValueString +
                   "' already exists";
        }

        //////////
        //  Operations
    public:
        //  The internal name of the object type.
        QString         objectTypeName() const { return _objectTypeName; }

        //  The name of the property.
        QString         propertyName() const { return _propertyName; }

        //  The string representation of the offending property value.
        QString         propertyValueString() const { return _propertyValueString; }

        //////////
        //  Implementation
    private:
        QString         _objectTypeName;
        QString         _propertyName;
        QString         _propertyValueString;
    };

    //  Thrown when an object with the specified property value,
    //  which is supposed to exist, does not.
    class TT3_WS_PUBLIC DoesNotExistException : public WorkspaceException
    {
        //////////
        //  Types
    public:
        using Self = DoesNotExistException;

        //////////
        //  Construction/destruction/assignment
    public:
        DoesNotExistException(const QString & objectTypeName,
                              const QString & propertyName, const QString & propertyValueString)
            :   _objectTypeName(objectTypeName),
                _propertyName(propertyName),
                _propertyValueString(propertyValueString) {}
        DoesNotExistException(ObjectType * objectType,
                              const QString & propertyName, const QString & propertyValueString)
            :   Self(objectType->displayName(), propertyName, propertyValueString) {}

        template <class T>
        DoesNotExistException(ObjectType * objectType,
                              const QString & propertyName, const T & propertyValue)
            :   Self(objectType->displayName(), propertyName, tt3::util::toString(propertyValue)) {}
        template <class T>
        DoesNotExistException(const QString & objectTypeName,
                              const QString & propertyName, const T & propertyValue)
            :   Self(objectTypeName, propertyName, tt3::util::toString(propertyValue)) {}

        //////////
        //  QException
    public:
        virtual Self *  clone() const { return new Self(*this); }
        virtual void    raise() const { throw *this; }

        //////////
        //  tt3::util::Exception
    public:
        virtual QString errorMessage() const override
        {
            return "The " + _objectTypeName +
                   " with '" + _propertyName +
                   "' = '" + _propertyValueString +
                   "' does not exist";
        }

        //////////
        //  Operations
    public:
        //  The internal name of the object type.
        QString         objectTypeName() const { return _objectTypeName; }

        //  The name of the property.
        QString         propertyName() const { return _propertyName; }

        //  The string representation of the offending property value.
        QString         propertyValueString() const { return _propertyValueString; }

        //////////
        //  Implementation
    private:
        QString         _objectTypeName;
        QString         _propertyName;
        QString         _propertyValueString;
    };

    //  Thrown when attempting to access a "dead" instance
    class TT3_WS_PUBLIC InstanceDeadException : public WorkspaceException
    {
        //////////
        //  Types
    public:
        using Self = InstanceDeadException;

        //////////
        //  Construction/destruction/assignment
    public:
        InstanceDeadException() = default;

        //////////
        //  QException
    public:
        virtual Self *  clone() const { return new Self(*this); }
        virtual void    raise() const { throw *this; }

        //////////
        //  tt3::util::Exception
    public:
        virtual QString errorMessage() const override
        {
            return "The instance is dead";
        }
    };

    //  Thrown when must carry a custom error message (from OS, etc.)
    class TT3_WS_PUBLIC CustomWorkspaceException : public WorkspaceException
    {
        //////////
        //  Types
    public:
        using Self = CustomWorkspaceException;

        //////////
        //  Construction/destruction/assignment
    public:
        explicit CustomWorkspaceException(const QString & errorMessage)
            :   _errorMessage(errorMessage) {}

        //////////
        //  QException
    public:
        virtual Self *  clone() const { return new Self(*this); }
        virtual void    raise() const { throw *this; }

        //////////
        //  tt3::util::Exception
    public:
        virtual QString errorMessage() const override
        {
            return _errorMessage;
        }

        //////////
        //  Implementation
    private:
        QString         _errorMessage;
    };
}

//  End of tt3-ws/DateTime.hpp
