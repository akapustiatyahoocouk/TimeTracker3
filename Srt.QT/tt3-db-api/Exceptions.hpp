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
    //  The common base class for all DB API - level exceptions
    class TT3_DB_API_PUBLIC DatabaseException : public tt3::util::Exception
    {
        //////////
        //  Construction/destruction/assignment
    public:
        DatabaseException();
    };

    //  Thrown when an invalid database address is specified
    class TT3_DB_API_PUBLIC InvalidDatabaseAddressException : public DatabaseException
    {
        //////////
        //  Types:
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

    //  Thrown when a single-user database is already in use
    class TT3_DB_API_PUBLIC DatabaseInUseException : public DatabaseException
    {
        //////////
        //  Types
    public:
        using Self = DatabaseInUseException;

        //////////
        //  Construction/destruction/assignment
    public:
        explicit DatabaseInUseException(IDatabaseAddress * databaseAddress);

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
        //  The user-readable display name of the database type
        QString         databaseTypeDisplayName() const { return _databaseTypeDisplayName; }

        //  Returns the user-readable display form of the
        //  address of the database that is already in use
        QString         databaseAddressDisplayForm() const { return _databaseAddressDisplayForm; }

        //////////
        //  Implementayion
    private:
        QString         _databaseTypeDisplayName;
        QString         _databaseAddressDisplayForm;
    };

    //  Thrown when a database is corrupt
    class TT3_DB_API_PUBLIC DatabaseCorruptException : public DatabaseException
    {
        //////////
        //  Types
    public:
        using Self = DatabaseCorruptException;

        //////////
        //  Construction/destruction/assignment
    public:
        explicit DatabaseCorruptException(IDatabaseAddress * databaseAddress);

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
        //  The user-readable display name of the database type
        QString         databaseTypeDisplayName() const { return _databaseTypeDisplayName; }

        //  Returns the user-readable display form of the
        //  address of the database that is already in use
        QString         databaseAddressDisplayForm() const { return _databaseAddressDisplayForm; }

        //////////
        //  Implementayion
    private:
        QString     _databaseTypeDisplayName;
        QString     _databaseAddressDisplayForm;
    };

    //  Thrown when a service is called on a closed database.
    class TT3_DB_API_PUBLIC DatabaseClosedException : public DatabaseException
    {
        //////////
        //  Types
    public:
        using Self = DatabaseClosedException;

        //////////
        //  Construction/destruction/assignment
    public:
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

    //  Thrown when a database service is access-denied
    class TT3_DB_API_PUBLIC AccessDeniedException : public DatabaseException
    {
        //////////
        //  Types
    public:
        using Self = AccessDeniedException;

        //////////
        //  Construction/destruction/assignment
    public:
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

    //  Thrown when attempting to assign an invalid value to a property
    class TT3_DB_API_PUBLIC InvalidPropertyValueException : public DatabaseException
    {
        //////////
        //  Types
    public:
        using Self = InvalidPropertyValueException;

        //////////
        //  Construction/destruction/assignment
    public:
        InvalidPropertyValueException(const QString & objectTypeName,
                                      const QString & propertyName, const QString & propertyValueString);
        InvalidPropertyValueException(IObjectType * objectType,
                                      const QString & propertyName, const QString & propertyValueString)
            :   Self(objectType->displayName(), propertyName, propertyValueString) {}

        template <class T>
        InvalidPropertyValueException(IObjectType * objectType,
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
        virtual QString errorMessage() const override;

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
    class TT3_DB_API_PUBLIC AlreadyExistsException : public DatabaseException
    {
        //////////
        //  Types
    public:
        using Self = AlreadyExistsException;

        //////////
        //  Construction/destruction/assignment
    public:
        AlreadyExistsException(const QString & objectTypeName,
                               const QString & propertyName, const QString & propertyValueString);
        AlreadyExistsException(IObjectType * objectType,
                               const QString & propertyName, const QString & propertyValueString)
            :   Self(objectType->displayName(), propertyName, propertyValueString) {}

        template <class T>
        AlreadyExistsException(IObjectType * objectType,
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
        virtual QString errorMessage() const override;

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
    class TT3_DB_API_PUBLIC DoesNotExistException : public DatabaseException
    {
        //////////
        //  Types
    public:
        using Self = DoesNotExistException;

        //////////
        //  Construction/destruction/assignment
    public:
        DoesNotExistException(const QString & objectTypeName,
                              const QString & propertyName, const QString & propertyValueString);
        DoesNotExistException(IObjectType * objectType,
                              const QString & propertyName, const QString & propertyValueString)
            :   Self(objectType->displayName(), propertyName, propertyValueString) {}

        template <class T>
        DoesNotExistException(IObjectType * objectType,
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
        virtual QString errorMessage() const override;

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
    class TT3_DB_API_PUBLIC InstanceDeadException : public DatabaseException
    {
        //////////
        //  Types
    public:
        using Self = InstanceDeadException;

        //////////
        //  Construction/destruction/assignment
    public:
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

    //  Thrown when must carry a custom error message (from OS, etc.)
    class TT3_DB_API_PUBLIC CustomDatabaseException : public DatabaseException
    {
        //////////
        //  Types
    public:
        using Self = CustomDatabaseException;

        //////////
        //  Construction/destruction/assignment
    public:
        explicit CustomDatabaseException(const QString & errorMessage);

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
