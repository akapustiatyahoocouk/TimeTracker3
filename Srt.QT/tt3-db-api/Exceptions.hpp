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
    //////////
    //  The common base class for all DB API - level exceptions
    class TT3_DB_API_PUBLIC DatabaseException : public tt3::util::Exception
    {
        //////////
        //  Construction/destruction/assignment
    public:
        DatabaseException()
            :   tt3::util::Exception("Unknown database error") {}
        explicit DatabaseException(const QString & errorMessage)
            :   tt3::util::Exception(errorMessage) {}

        //////////
        //  QException
    public:
        virtual DatabaseException *
                        clone() const { return new DatabaseException(*this); }
        virtual void    raise() const { throw *this; }
    };

    //  Thrown when an invalid database address is specified
    class TT3_DB_API_PUBLIC InvalidDatabaseAddressException : public DatabaseException
    {
        //////////
        //  Construction/destruction/assignment
    public:
        InvalidDatabaseAddressException()
            :   DatabaseException("Invalid database address") {}

        //////////
        //  QException
    public:
        virtual InvalidDatabaseAddressException *
                        clone() const { return new InvalidDatabaseAddressException(*this); }
        virtual void    raise() const { throw *this; }
    };

    //  Thrown when a single-user database is already in use
    class TT3_DB_API_PUBLIC DatabaseInUseException : public DatabaseException
    {
        //////////
        //  Construction/destruction/assignment
    public:
        explicit DatabaseInUseException(IDatabaseAddress * address)
            :   DatabaseException(address->databaseType()->displayName() + " "  +
                                  address->displayForm() + " is already in use") {}

        //////////
        //  QException
    public:
        virtual DatabaseInUseException *
                        clone() const { return new DatabaseInUseException(*this); }
        virtual void    raise() const { throw *this; }
    };

    //  Thrown when a database is corrupt
    class TT3_DB_API_PUBLIC DatabaseCorruptException : public DatabaseException
    {
        //////////
        //  Construction/destruction/assignment
    public:
        explicit DatabaseCorruptException(IDatabaseAddress * address)
            :   DatabaseException(address->databaseType()->displayName() + " "  +
                                  address->displayForm() + " is corrupt") {}

        //////////
        //  QException
    public:
        virtual DatabaseCorruptException *
                        clone() const { return new DatabaseCorruptException(*this); }
        virtual void    raise() const { throw *this; }
    };

    //  Thrown when a service is called on a closed database.
    class TT3_DB_API_PUBLIC DatabaseClosedException : public DatabaseException
    {
        //////////
        //  Construction/destruction/assignment
    public:
        explicit DatabaseClosedException()
            :   DatabaseException("The database is closed") {}

        //////////
        //  QException
    public:
        virtual DatabaseClosedException *
                        clone() const { return new DatabaseClosedException(*this); }
        virtual void    raise() const { throw *this; }
    };

    //  Thrown when a workspace service is access-denied
    class TT3_DB_API_PUBLIC AccessDeniedException : public DatabaseException
    {
        //////////
        //  Construction/destruction/assignment
    public:
        AccessDeniedException()
            :   DatabaseException("Access denied") {}

        //////////
        //  QException
    public:
        virtual AccessDeniedException *
                        clone() const { return new AccessDeniedException(*this); }
        virtual void    raise() const { throw *this; }
    };

    //  Thrown when attempting to assign an invalid value to a property
    class TT3_DB_API_PUBLIC InvalidPropertyValueException : public DatabaseException
    {
        //////////
        //  Construction/destruction/assignment
    public:
        InvalidPropertyValueException(const QString & objectTypeName,
                                      const QString & propertyName, const QString & propertyValue)
            :   DatabaseException("Property '" + propertyName +
                                "' of '" + objectTypeName +
                                "' cannot be set to '" + propertyValue + "'") {}
        InvalidPropertyValueException(IObjectType * objectType,
                                      const QString & propertyName, const QString & propertyValue)
            :   InvalidPropertyValueException(objectType->displayName(), propertyName, propertyValue) {}

        template <class T>
        InvalidPropertyValueException(IObjectType * objectType,
                                      const QString & propertyName, const T & propertyValue)
            :   InvalidPropertyValueException(objectType->displayName(), propertyName, tt3::util::toString(propertyValue)) {}
        template <class T>
        InvalidPropertyValueException(const QString & objectTypeName,
                                      const QString & propertyName, const T & propertyValue)
            :   InvalidPropertyValueException(objectTypeName, propertyName, tt3::util::toString(propertyValue)) {}

        //////////
        //  QException
    public:
        virtual InvalidPropertyValueException *
                        clone() const { return new InvalidPropertyValueException(*this); }
        virtual void    raise() const { throw *this; }
    };

    //  Thrown when an object with the specified property value,
    //  which is supposed to be unique, already exists
    class TT3_DB_API_PUBLIC AlreadyExistsException : public DatabaseException
    {
        //////////
        //  Types
    public:
        //  TODO use for all exceptions?
        using Self = AlreadyExistsException;

        //////////
        //  Construction/destruction/assignment
    public:
        AlreadyExistsException(const QString & objectTypeName,
                               const QString & propertyName, const QString & propertyValue)
            :   DatabaseException("The " + objectTypeName +
                                " with '" + propertyName +
                                "' = '" + propertyValue +
                                "' already exists") {}
        AlreadyExistsException(IObjectType * objectType,
                               const QString & propertyName, const QString & propertyValue)
            :   AlreadyExistsException(objectType->displayName(), propertyName, propertyValue) {}

        template <class T>
        AlreadyExistsException(IObjectType * objectType,
                              const QString & propertyName, const T & propertyValue)
            :   AlreadyExistsException(objectType->displayName(), propertyName, tt3::util::toString(propertyValue)) {}
        template <class T>
        AlreadyExistsException(const QString & objectTypeName,
                               const QString & propertyName, const T & propertyValue)
            :   AlreadyExistsException(objectTypeName, propertyName, tt3::util::toString(propertyValue)) {}

        //////////
        //  QException
    public:
        virtual Self *  clone() const { return new Self(*this); }
        virtual void    raise() const { throw *this; }
    };

    //  Thrown when attempting to access a "dead" instance
    class TT3_DB_API_PUBLIC InstanceDeadException : public DatabaseException
    {
        //////////
        //  Construction/destruction/assignment
    public:
        InstanceDeadException()
            :   DatabaseException("Instance is dead") {}

        //////////
        //  QException
    public:
        virtual InstanceDeadException *
                        clone() const { return new InstanceDeadException(*this); }
        virtual void    raise() const { throw *this; }
    };
    }

//  End of tt3-db-api/Exceptions.hpp
