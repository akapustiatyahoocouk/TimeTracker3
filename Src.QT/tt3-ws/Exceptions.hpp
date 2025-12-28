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
    /// \class WorkspaceException tt3-ws/API.hpp
    /// \brief The common base class for all workspace - level exceptions.
    class TT3_WS_PUBLIC WorkspaceException
        :   public tt3::util::Exception
    {
        //////////
        //  Construction/destruction/assignment
    public:
        /// \brief
        ///     Constructs the exception.
        WorkspaceException();

        //////////
        //  Operations
    public:
        /// \brief
        ///     Throws WorkspaceException (or a subclass thereof)
        ///     that best represents the specified arbitrary exception
        /// \param ex
        ///     The exception to translate and re-throw.
        /// \exception WorkspaceException
        ///     Always.
        Q_NORETURN
        static void     translateAndThrow(
                            const tt3::util::Exception & ex);
    };

    /// \class InvalidWorkspaceAddressException tt3-ws/API.hpp
    /// \brief Thrown when an invalid workspace address is specified.
    class TT3_WS_PUBLIC InvalidWorkspaceAddressException
        :   public WorkspaceException
    {
        //////////
        //  Types:
    public:
        /// \brief A type alias to improve code readability.
        using Self = InvalidWorkspaceAddressException;

        //////////
        //  Construction/destruction/assignment
    public:
        /// \brief
        ///     Constructs the exception.
        InvalidWorkspaceAddressException();

        //////////
        //  QException
    public:
        virtual Self *  clone() const override { return new Self(*this); }
        virtual void    raise() const override { throw *this; }

        //////////
        //  tt3::util::Exception
    public:
        virtual QString errorMessage() const override;
    };

    /// \class WorkspaceInUseException tt3-ws/API.hpp
    /// \brief Thrown when a single-user workspace is already in use.
    class TT3_WS_PUBLIC WorkspaceInUseException
        :   public WorkspaceException
    {
        friend class WorkspaceException;

        //////////
        //  Types
    public:
        /// \brief A type alias to improve code readability.
        using Self = WorkspaceInUseException;

        //////////
        //  Construction/destruction/assignment
    private:
        WorkspaceInUseException(
                const QString & workspaceTypeDisplayName,
                const QString & workspaceAddressDisplayForm
            );
    public:
        /// \brief
        ///     Constructs the exception.
        /// \param workspaceAddress
        ///     The address of the workspace in question.
        explicit WorkspaceInUseException(
                const WorkspaceAddress & workspaceAddress
            );

        //////////
        //  QException
    public:
        virtual Self *  clone() const override { return new Self(*this); }
        virtual void    raise() const override { throw *this; }

        //////////
        //  tt3::util::Exception
    public:
        virtual QString errorMessage() const override;

        //////////
        //  Operations
    public:
        /// \brief
        ///     Returns the user-readable display name of the workspace type.
        /// \return
        ///     The user-readable display name of the workspace type.
        QString         workspaceTypeDisplayName() const { return _workspaceTypeDisplayName; }

        /// \brief
        ///     Returns the user-readable display form of the
        ///     address of the workspace that is already in use.
        /// \return
        ///     The user-readable display form of the
        ///     address of the workspace that is already in use.
        QString         workspaceAddressDisplayForm() const { return _workspaceAddressDisplayForm; }

        //////////
        //  Implementayion
    private:
        QString         _workspaceTypeDisplayName;
        QString         _workspaceAddressDisplayForm;
    };

    /// \class WorkspaceCorruptException tt3-ws/API.hpp
    /// \brief Thrown when a workspace is corrupt.
    class TT3_WS_PUBLIC WorkspaceCorruptException
        :   public WorkspaceException
    {
        friend class WorkspaceException;

        //////////
        //  Types
    public:
        /// \brief A type alias to improve code readability.
        using Self = WorkspaceCorruptException;

        //////////
        //  Construction/destruction/assignment
    private:
        WorkspaceCorruptException(
                const QString & workspaceTypeDisplayName,
                const QString & workspaceAddressDisplayForm
            );
    public:
        /// \brief
        ///     Constructs the exception.
        /// \param workspaceAddress
        ///     The address of the workspace in question.
        explicit WorkspaceCorruptException(
                const WorkspaceAddress & workspaceAddress
            );

        //////////
        //  QException
    public:
        virtual Self *  clone() const override { return new Self(*this); }
        virtual void    raise() const override { throw *this; }

        //////////
        //  tt3::util::Exception
    public:
        virtual QString errorMessage() const override;

        //////////
        //  Operations
    public:
        /// \brief
        ///     Returns the user-readable display name of the workspace type.
        /// \return
        ///     The user-readable display name of the workspace type.
        QString         workspaceTypeDisplayName() const { return _workspaceTypeDisplayName; }

        /// \brief
        ///     Returns the user-readable display form of the
        ///     address of the workspace that is corrupt.
        /// \return
        ///     The user-readable display form of the
        ///     address of the workspace that is corrupt.
        QString         workspaceAddressDisplayForm() const { return _workspaceAddressDisplayForm; }

        //////////
        //  Implementayion
    private:
        QString         _workspaceTypeDisplayName;
        QString         _workspaceAddressDisplayForm;
    };

    /// \class WorkspaceClosedException tt3-ws/API.hpp
    /// \brief Thrown when a service is called on a closed Workspace.
    class TT3_WS_PUBLIC WorkspaceClosedException
        :   public WorkspaceException
    {
        //////////
        //  Types
    public:
        /// \brief A type alias to improve code readability.
        using Self = WorkspaceClosedException;

        //////////
        //  Construction/destruction/assignment
    public:
        /// \brief
        ///     Constructs the exception.
        WorkspaceClosedException();

        //////////
        //  QException
    public:
        virtual Self *  clone() const override { return new Self(*this); }
        virtual void    raise() const override { throw *this; }

        //////////
        //  tt3::util::Exception
    public:
        virtual QString errorMessage() const override;
    };

    /// \class AccessDeniedException tt3-ws/API.hpp
    /// \brief Thrown when a workspace service is access-denied.
    class TT3_WS_PUBLIC AccessDeniedException
        :   public WorkspaceException
    {
        //////////
        //  Types
    public:
        /// \brief A type alias to improve code readability.
        using Self = AccessDeniedException;

        //////////
        //  Construction/destruction/assignment
    public:
        /// \brief
        ///     Constructs the exception.
        AccessDeniedException();

        //////////
        //  QException
    public:
        virtual Self *  clone() const override { return new Self(*this); }
        virtual void    raise() const override { throw *this; }

        //////////
        //  tt3::util::Exception
    public:
        virtual QString errorMessage() const override;
    };

    /// \class InvalidPropertyValueException tt3-ws/API.hpp
    /// \brief Thrown when attempting to assign an invalid value to a property.
    class TT3_WS_PUBLIC InvalidPropertyValueException
        :   public WorkspaceException
    {
        //////////
        //  Types
    public:
        /// \brief A type alias to improve code readability.
        using Self = InvalidPropertyValueException;

        //////////
        //  Construction/destruction/assignment
    public:
        /// \brief
        ///     Constructs the exception.
        /// \param objectTypeName
        ///     The internal name of the object's type.
        /// \param propertyName
        ///     The name of the property.
        /// \param propertyValueString
        ///     The string form of the offending property value.
        InvalidPropertyValueException(
                const QString & objectTypeName,
                const QString & propertyName,
                const QString & propertyValueString
            );

        /// \brief
        ///     Constructs the exception.
        /// \param objectType
        ///     The object's type.
        /// \param propertyName
        ///     The name of the property.
        /// \param propertyValueString
        ///     The string form of the offending property value.
        InvalidPropertyValueException(
                ObjectType * objectType,
                const QString & propertyName,
                const QString & propertyValueString
            ) : Self(objectType->displayName(), propertyName, propertyValueString) {}

        /// \brief
        ///     Constructs the exception.
        /// \param objectType
        ///     The object's type.
        /// \param propertyName
        ///     The name of the property.
        /// \param propertyValue
        ///     The offending property value.
        template <class T>
        InvalidPropertyValueException(
                ObjectType * objectType,
                const QString & propertyName,
                const T & propertyValue
            ) : Self(objectType->displayName(), propertyName, tt3::util::toString(propertyValue)) {}

        /// \brief
        ///     Constructs the exception.
        /// \param objectTypeName
        ///     The internal name of the object's type.
        /// \param propertyName
        ///     The name of the property.
        /// \param propertyValue
        ///     The offending property value.
        template <class T>
        InvalidPropertyValueException(
                const QString & objectTypeName,
                const QString & propertyName,
                const T & propertyValue
            ) : Self(objectTypeName, propertyName, tt3::util::toString(propertyValue)) {}

        //////////
        //  QException
    public:
        virtual Self *  clone() const override { return new Self(*this); }
        virtual void    raise() const override { throw *this; }

        //////////
        //  tt3::util::Exception
    public:
        virtual QString errorMessage() const override;

        //////////
        //  Operations
    public:
        /// \brief
        ///     Returns the internal name of the object type.
        /// \return
        ///     The internal name of the object type.
        QString         objectTypeName() const { return _objectTypeName; }

        /// \brief
        ///     Returns the name of the property.
        /// \return
        ///     The name of the property.
        QString         propertyName() const { return _propertyName; }

        /// \brief
        ///     Returns the string representation of the offending property value.
        /// \return
        ///     The string representation of the offending property value.
        QString         propertyValueString() const { return _propertyValueString; }

        //////////
        //  Implementation
    private:
        QString         _objectTypeName;
        QString         _propertyName;
        QString         _propertyValueString;
    };

    /// \class AlreadyExistsException tt3-ws/API.hpp
    /// \brief
    ///     Thrown when an object with the specified property value,
    ///     which is supposed to be unique, already exists.
    class TT3_WS_PUBLIC AlreadyExistsException
        :   public WorkspaceException
    {
        //////////
        //  Types
    public:
        /// \brief A type alias to improve code readability.
        using Self = AlreadyExistsException;

        //////////
        //  Construction/destruction/assignment
    public:
        /// \brief
        ///     Constructs the exception.
        /// \param objectTypeName
        ///     The internal name of the object's type.
        /// \param propertyName
        ///     The name of the unique property.
        /// \param propertyValueString
        ///     The string form of the duplicate property value.
        AlreadyExistsException(
                const QString & objectTypeName,
                const QString & propertyName,
                const QString & propertyValueString
            );

        /// \brief
        ///     Constructs the exception.
        /// \param objectType
        ///     The object's type.
        /// \param propertyName
        ///     The name of the unique property.
        /// \param propertyValueString
        ///     The string form of the duplicate property value.
        AlreadyExistsException(
                ObjectType * objectType,
                const QString & propertyName,
                const QString & propertyValueString
            ) : Self(objectType->displayName(), propertyName, propertyValueString) {}

        /// \brief
        ///     Constructs the exception.
        /// \param objectType
        ///     The object's type.
        /// \param propertyName
        ///     The name of the unique property.
        /// \param propertyValue
        ///     The duplicate property value.
        template <class T>
        AlreadyExistsException(
                ObjectType * objectType,
                const QString & propertyName,
                const T & propertyValue
            ) : Self(objectType->displayName(), propertyName, tt3::util::toString(propertyValue)) {}

        /// \brief
        ///     Constructs the exception.
        /// \param objectTypeName
        ///     The internal name of the object's type.
        /// \param propertyName
        ///     The name of the unique property.
        /// \param propertyValue
        ///     The duplicate property value.
        template <class T>
        AlreadyExistsException(
                const QString & objectTypeName,
                const QString & propertyName,
                const T & propertyValue
            ) : Self(objectTypeName, propertyName, tt3::util::toString(propertyValue)) {}

        //////////
        //  QException
    public:
        virtual Self *  clone() const override { return new Self(*this); }
        virtual void    raise() const override { throw *this; }

        //////////
        //  tt3::util::Exception
    public:
        virtual QString errorMessage() const override;

        //////////
        //  Operations
    public:
        /// \brief
        ///     Returns the internal name of the object type.
        /// \return
        ///     The internal name of the object type.
        QString         objectTypeName() const { return _objectTypeName; }

        /// \brief
        ///     Returns the name of the unique property.
        /// \return
        ///     The name of the unique property.
        QString         propertyName() const { return _propertyName; }

        /// \brief
        ///     Returns the string representation of the duplicate property value.
        /// \return
        ///     The string representation of the duplicate property value.
        QString         propertyValueString() const { return _propertyValueString; }

        //////////
        //  Implementation
    private:
        QString         _objectTypeName;
        QString         _propertyName;
        QString         _propertyValueString;
    };

    /// \class DoesNotExistException tt3-ws/API.hpp
    /// \brief
    ///     Thrown when an object with the specified property value,
    ///     which is supposed to exist, does not.
    class TT3_WS_PUBLIC DoesNotExistException
        :   public WorkspaceException
    {
        //////////
        //  Types
    public:
        /// \brief A type alias to improve code readability.
        using Self = DoesNotExistException;

        //////////
        //  Construction/destruction/assignment
    public:
        /// \brief
        ///     Constructs the exception.
        /// \param objectTypeName
        ///     The internal name of the object's type.
        /// \param propertyName
        ///     The name of the property.
        /// \param propertyValueString
        ///     The string form of the absent property value.
        DoesNotExistException(
                const QString & objectTypeName,
                const QString & propertyName,
                const QString & propertyValueString
            );

        /// \brief
        ///     Constructs the exception.
        /// \param objectType
        ///     The object's type.
        /// \param propertyName
        ///     The name of the property.
        /// \param propertyValueString
        ///     The string form of the absent property value.
        DoesNotExistException(
                ObjectType * objectType,
                const QString & propertyName,
                const QString & propertyValueString
            ) : Self(objectType->displayName(), propertyName, propertyValueString) {}

        /// \brief
        ///     Constructs the exception.
        /// \param objectType
        ///     The object's type.
        /// \param propertyName
        ///     The name of the property.
        /// \param propertyValue
        ///     The absent property value.
        template <class T>
        DoesNotExistException(
                ObjectType * objectType,
                const QString & propertyName,
                const T & propertyValue
            ) : Self(objectType->displayName(), propertyName, tt3::util::toString(propertyValue)) {}

        /// \brief
        ///     Constructs the exception.
        /// \param objectTypeName
        ///     The internal name of the object's type.
        /// \param propertyName
        ///     The name of the property.
        /// \param propertyValue
        ///     The absent property value.
        template <class T>
        DoesNotExistException(
                const QString & objectTypeName,
                const QString & propertyName,
                const T & propertyValue
            ) : Self(objectTypeName, propertyName, tt3::util::toString(propertyValue)) {}

        //////////
        //  QException
    public:
        virtual Self *  clone() const override { return new Self(*this); }
        virtual void    raise() const override { throw *this; }

        //////////
        //  tt3::util::Exception
    public:
        virtual QString errorMessage() const override;

        //////////
        //  Operations
    public:
        /// \brief
        ///     Returns the internal name of the object type.
        /// \return
        ///     The internal name of the object type.
        QString         objectTypeName() const { return _objectTypeName; }

        /// \brief
        ///     Returns the name of the property.
        /// \return
        ///     The name of the property.
        QString         propertyName() const { return _propertyName; }

        /// \brief
        ///     Returns the string representation of the absent property value.
        /// \return
        ///     The string representation of the absent property value.
        QString         propertyValueString() const { return _propertyValueString; }

        //////////
        //  Implementation
    private:
        QString         _objectTypeName;
        QString         _propertyName;
        QString         _propertyValueString;
    };

    /// \class InstanceDeadException tt3-ws/API.hpp
    /// \brief Thrown when attempting to access a "dead" instance.
    class TT3_WS_PUBLIC InstanceDeadException
        :   public WorkspaceException
    {
        //////////
        //  Types
    public:
        /// \brief A type alias to improve code readability.
        using Self = InstanceDeadException;

        //////////
        //  Construction/destruction/assignment
    public:
        /// \brief
        ///     Constructs the exception.
        InstanceDeadException();

        //////////
        //  QException
    public:
        virtual Self *  clone() const override { return new Self(*this); }
        virtual void    raise() const override { throw *this; }

        //////////
        //  tt3::util::Exception
    public:
        virtual QString errorMessage() const override;
    };

    /// \class AccessWouldBeLostException tt3-ws/API.hpp
    /// \brief
    ///     Thrown when attempting to leave workspace without
    ///     an enbled administrator user+account.
    class TT3_WS_PUBLIC AccessWouldBeLostException
        :   public WorkspaceException
    {
        //////////
        //  Types
    public:
        /// \brief A type alias to improve code readability.
        using Self = AccessWouldBeLostException;

        //////////
        //  Construction/destruction/assignment
    public:
        /// \brief
        ///     Constructs the exception.
        AccessWouldBeLostException();

        //////////
        //  QException
    public:
        virtual Self *  clone() const override { return new Self(*this); }
        virtual void    raise() const override { throw *this; }

        //////////
        //  tt3::util::Exception
    public:
        virtual QString errorMessage() const override;
    };

    /// \class IncompatibleInstanceException tt3-ws/API.hpp
    /// \brief Thrown when passing an incompatible instance to a service.
    class TT3_WS_PUBLIC IncompatibleInstanceException
        :   public WorkspaceException
    {
        //////////
        //  Types
    public:
        /// \brief A type alias to improve code readability.
        using Self = IncompatibleInstanceException;

        //////////
        //  Construction/destruction/assignment
    public:
        /// \brief
        ///     Constructs the exception.
        /// \param objectTypeName
        ///     The name of the incompatible instance' type.
        explicit IncompatibleInstanceException(
                const QString & objectTypeName
            );

        /// \brief
        ///     Constructs the exception.
        /// \param objectType
        ///     The incompatible instance' type.
        explicit IncompatibleInstanceException(
                ObjectType * objectType
            ) : Self(objectType->displayName()) {}

        //////////
        //  QException
    public:
        virtual Self *  clone() const override { return new Self(*this); }
        virtual void    raise() const override { throw *this; }

        //////////
        //  tt3::util::Exception
    public:
        virtual QString errorMessage() const override;

        //////////
        //  Operations
    public:
        /// \brief
        ///     Returns the internal name of the object type.
        /// \return
        ///     The internal name of the object type.
        QString         objectTypeName() const { return _objectTypeName; }

        //////////
        //  Implementation
    private:
        QString         _objectTypeName;
    };

    /// \class CustomWorkspaceException tt3-ws/API.hpp
    /// \brief Thrown when must carry a custom error message (from OS, etc.)
    class TT3_WS_PUBLIC CustomWorkspaceException
        :   public WorkspaceException
    {
        //////////
        //  Types
    public:
        /// \brief A type alias to improve code readability.
        using Self = CustomWorkspaceException;

        //////////
        //  Construction/destruction/assignment
    public:
        /// \brief
        ///     Constructs the exception.
        /// \param errorMessage
        ///     The custom error message.
        explicit CustomWorkspaceException(
                const QString & errorMessage
            );

        //////////
        //  QException
    public:
        virtual Self *  clone() const override { return new Self(*this); }
        virtual void    raise() const override { throw *this; }

        //////////
        //  tt3::util::Exception
    public:
        virtual QString errorMessage() const override;

        //////////
        //  Implementation
    private:
        QString         _errorMessage;
    };
}

//  End of tt3-ws/DateTime.hpp
