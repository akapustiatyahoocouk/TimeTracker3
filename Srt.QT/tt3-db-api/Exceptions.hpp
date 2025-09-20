//
//  tt3-db-api/Exceptions.hpp - DB API - level exceptions
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

namespace tt3::db::api
{
    /// \class DatabaseException tt3-db-api/API.hpp
    /// \brief The common base class for all DB API - level exceptions.
    class TT3_DB_API_PUBLIC DatabaseException
        :   public tt3::util::Exception
    {
        //////////
        //  Construction/destruction/assignment
    public:
        /// \brief
        ///     Constructs the exception.
        DatabaseException();
    };

    /// \class InvalidDatabaseAddressException tt3-db-api/API.hpp
    /// \brief Thrown when an invalid database address is specified.
    class TT3_DB_API_PUBLIC InvalidDatabaseAddressException
        :   public DatabaseException
    {
        //////////
        //  Types
    public:
        /// \brief A type alias to improve code readability.
        using Self = InvalidDatabaseAddressException;

        //////////
        //  Construction/destruction/assignment
    public:
        InvalidDatabaseAddressException();

        //////////
        //  QException
    public:
        virtual Self *  clone() const { return new Self(*this); }
        virtual void    raise() const { throw *this; }

        //////////
        //  tt3::util::Exception
    public:
        virtual QString errorMessage() const override;
    };

    /// \class DatabaseInUseException tt3-db-api/API.hpp
    /// \brief Thrown when a single-user database is already in use.
    class TT3_DB_API_PUBLIC DatabaseInUseException
        :   public DatabaseException
    {
        //////////
        //  Types
    public:
        /// \brief A type alias to improve code readability.
        using Self = DatabaseInUseException;

        //////////
        //  Construction/destruction/assignment
    public:
        /// \brief
        ///     Constructs the exception.
        /// \param databaseAddress
        ///     The address of the offending database.
        explicit DatabaseInUseException(
                IDatabaseAddress * databaseAddress
            );

        //////////
        //  QException
    public:
        virtual Self *  clone() const { return new Self(*this); }
        virtual void    raise() const { throw *this; }

        //////////
        //  tt3::util::Exception
    public:
        virtual QString errorMessage() const override;

        //////////
        //  Operations
    public:
        /// \brief
        ///     Returns the user-readable display name of the database type.
        /// \return
        ///     The user-readable display name of the database type.
        QString         databaseTypeDisplayName() const { return _databaseTypeDisplayName; }

        /// \brief
        ///     Returns the user-readable display form of the
        ///     address of the database that is already in use.
        /// \return
        ///     The user-readable display form of the
        ///     address of the database that is already in use.
        QString         databaseAddressDisplayForm() const { return _databaseAddressDisplayForm; }

        //////////
        //  Implementayion
    private:
        QString         _databaseTypeDisplayName;
        QString         _databaseAddressDisplayForm;
    };

    /// \class DatabaseCorruptException tt3-db-api/API.hpp
    /// \brief Thrown when a database is corrupt.
    class TT3_DB_API_PUBLIC DatabaseCorruptException
        :   public DatabaseException
    {
        //////////
        //  Types
    public:
        /// \brief A type alias to improve code readability.
        using Self = DatabaseCorruptException;

        //////////
        //  Construction/destruction/assignment
    public:
        /// \brief
        ///     Constructs the exception.
        /// \param databaseAddress
        ///     The address of the offending database.
        explicit DatabaseCorruptException(
                IDatabaseAddress * databaseAddress
            );

        //////////
        //  QException
    public:
        virtual Self *  clone() const { return new Self(*this); }
        virtual void    raise() const { throw *this; }

        //////////
        //  tt3::util::Exception
    public:
        virtual QString errorMessage() const override;

        //////////
        //  Operations
    public:
        /// \brief
        ///     Returns the user-readable display name of the database type.
        /// \return
        ///     The user-readable display name of the database type.
        QString         databaseTypeDisplayName() const { return _databaseTypeDisplayName; }

        /// \brief
        ///     Returns the user-readable display form of the.
        ///     address of the database that is corrupt.
        /// \return
        ///     The user-readable display form of the.
        ///     address of the database that is corrupt.
        QString         databaseAddressDisplayForm() const { return _databaseAddressDisplayForm; }

        //////////
        //  Implementayion
    private:
        QString         _databaseTypeDisplayName;
        QString         _databaseAddressDisplayForm;
    };

    /// \class DatabaseClosedException tt3-db-api/API.hpp
    /// \brief Thrown when a service is called on a closed database.
    class TT3_DB_API_PUBLIC DatabaseClosedException
        :   public DatabaseException
    {
        //////////
        //  Types
    public:
        /// \brief A type alias to improve code readability.
        using Self = DatabaseClosedException;

        //////////
        //  Construction/destruction/assignment
    public:
        /// \brief
        ///     Constructs the exception.
        DatabaseClosedException();

        //////////
        //  QException
    public:
        virtual Self *  clone() const { return new Self(*this); }
        virtual void    raise() const { throw *this; }

        //////////
        //  tt3::util::Exception
    public:
        virtual QString errorMessage() const override;
    };

    /// \class AccessDeniedException tt3-db-api/API.hpp
    /// \brief Thrown when a database service is access-denied
    class TT3_DB_API_PUBLIC AccessDeniedException
        :   public DatabaseException
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
        virtual Self *  clone() const { return new Self(*this); }
        virtual void    raise() const { throw *this; }

        //////////
        //  tt3::util::Exception
    public:
        virtual QString errorMessage() const override;
    };

    /// \class InvalidPropertyValueException tt3-db-api/API.hpp
    /// \brief Thrown when attempting to assign an invalid value to a property.
    class TT3_DB_API_PUBLIC InvalidPropertyValueException
        :   public DatabaseException
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
        ///     The name of the object's type.
        /// \param propertyName
        ///     The name of the object's property.
        /// \param propertyValueString
        ///     The string representation of the offending property value.
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
        ///     The name of the object's property.
        /// \param propertyValueString
        ///     The string representation of the offending property value.
        InvalidPropertyValueException(
                IObjectType * objectType,
                const QString & propertyName,
                const QString & propertyValueString
            ) : Self(objectType->displayName(), propertyName, propertyValueString) {}

        /// \brief
        ///     Constructs the exception.
        /// \param objectTypeName
        ///     The name of the object's type.
        /// \param propertyName
        ///     The name of the object's property.
        /// \param propertyValue
        ///     The offending property value.
        template <class T>
        InvalidPropertyValueException(
                const QString & objectTypeName,
                const QString & propertyName,
                const T & propertyValue
            ) : Self(objectTypeName, propertyName, tt3::util::toString(propertyValue)) {}

        /// \brief
        ///     Constructs the exception.
        /// \param objectType
        ///     The object's type.
        /// \param propertyName
        ///     The name of the object's property.
        /// \param propertyValue
        ///     The offending property value.
        template <class T>
        InvalidPropertyValueException(
                IObjectType * objectType,
                const QString & propertyName,
                const T & propertyValue
            ) : Self(objectType->displayName(), propertyName, tt3::util::toString(propertyValue)) {}

        //////////
        //  QException
    public:
        virtual Self *  clone() const { return new Self(*this); }
        virtual void    raise() const { throw *this; }

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

    /// \class AlreadyExistsException tt3-db-api/API.hpp
    /// \brief
    ///     Thrown when an object with the specified property value,
    ///     which is supposed to be unique, already exists.
    class TT3_DB_API_PUBLIC AlreadyExistsException
        :   public DatabaseException
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
        ///     The name of the object's type.
        /// \param propertyName
        ///     The name of the object's property.
        /// \param propertyValueString
        ///     The string representation of the offending property value.
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
        ///     The name of the object's property.
        /// \param propertyValueString
        ///     The string representation of the offending property value.
        AlreadyExistsException(
                IObjectType * objectType,
                const QString & propertyName,
                const QString & propertyValueString
            ) : Self(objectType->displayName(), propertyName, propertyValueString) {}

        /// \brief
        ///     Constructs the exception.
        /// \param objectTypeName
        ///     The name of the object's type.
        /// \param propertyName
        ///     The name of the object's property.
        /// \param propertyValue
        ///     The offending property value.
        template <class T>
        AlreadyExistsException(
                const QString & objectTypeName,
                const QString & propertyName,
                const T & propertyValue
            ) : Self(objectTypeName, propertyName, tt3::util::toString(propertyValue)) {}

        /// \brief
        ///     Constructs the exception.
        /// \param objectType
        ///     The object's type.
        /// \param propertyName
        ///     The name of the object's property.
        /// \param propertyValue
        ///     The offending property value.
        template <class T>
        AlreadyExistsException(
                IObjectType * objectType,
                const QString & propertyName,
                const T & propertyValue
            ) : Self(objectType->displayName(), propertyName, tt3::util::toString(propertyValue)) {}

        //////////
        //  QException
    public:
        virtual Self *  clone() const { return new Self(*this); }
        virtual void    raise() const { throw *this; }

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

    /// \class DoesNotExistException tt3-db-api/API.hpp
    /// \brief
    ///     Thrown when an object with the specified property value,
    ///     which is supposed to exist, does not.
    class TT3_DB_API_PUBLIC DoesNotExistException
        :   public DatabaseException
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
        ///     The name of the object's type.
        /// \param propertyName
        ///     The name of the object's property.
        /// \param propertyValueString
        ///     The string representation of the offending property value.
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
        ///     The name of the object's property.
        /// \param propertyValueString
        ///     The string representation of the offending property value.
        DoesNotExistException(
                IObjectType * objectType,
                const QString & propertyName,
                const QString & propertyValueString
            ) : Self(objectType->displayName(), propertyName, propertyValueString) {}

        /// \brief
        ///     Constructs the exception.
        /// \param objectTypeName
        ///     The name of the object's type.
        /// \param propertyName
        ///     The name of the object's property.
        /// \param propertyValue
        ///     The offending property value.
        template <class T>
        DoesNotExistException(
                const QString & objectTypeName,
                const QString & propertyName,
                const T & propertyValue
            ) : Self(objectTypeName, propertyName, tt3::util::toString(propertyValue)) {}

        /// \brief
        ///     Constructs the exception.
        /// \param objectType
        ///     The object's type.
        /// \param propertyName
        ///     The name of the object's property.
        /// \param propertyValue
        ///     The offending property value.
        template <class T>
        DoesNotExistException(
                IObjectType * objectType,
                const QString & propertyName,
                const T & propertyValue
            ) : Self(objectType->displayName(), propertyName, tt3::util::toString(propertyValue)) {}

        //////////
        //  QException
    public:
        virtual Self *  clone() const { return new Self(*this); }
        virtual void    raise() const { throw *this; }

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

    /// \class InstanceDeadException tt3-db-api/API.hpp
    /// \brief Thrown when attempting to access a "dead" instance.
    class TT3_DB_API_PUBLIC InstanceDeadException
        :   public DatabaseException
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
        virtual Self *  clone() const { return new Self(*this); }
        virtual void    raise() const { throw *this; }

        //////////
        //  tt3::util::Exception
    public:
        virtual QString errorMessage() const override;
    };

    /// \class IncompatibleInstanceException tt3-db-api/API.hpp
    /// \brief Thrown when passing an incompatible instance to a service.
    class TT3_DB_API_PUBLIC IncompatibleInstanceException
        :   public DatabaseException
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
                IObjectType * objectType
            ) : Self(objectType->displayName()) {}

        //////////
        //  QException
    public:
        virtual Self *  clone() const { return new Self(*this); }
        virtual void    raise() const { throw *this; }

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

    /// \class CustomDatabaseException tt3-db-api/API.hpp
    /// \brief Thrown when must carry a custom error message (from OS, etc.)
    class TT3_DB_API_PUBLIC CustomDatabaseException
        :   public DatabaseException
    {
        //////////
        //  Types
    public:
        /// \brief A type alias to improve code readability.
        using Self = CustomDatabaseException;

        //////////
        //  Construction/destruction/assignment
    public:
        /// \brief
        ///     Constructs the exception.
        /// \param cerrorMessage
        ///     The custom error message.
        explicit CustomDatabaseException(
                const QString & cerrorMessage
            );

        //////////
        //  QException
    public:
        virtual Self *  clone() const { return new Self(*this); }
        virtual void    raise() const { throw *this; }

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

//  End of tt3-db-api/Exceptions.hpp
