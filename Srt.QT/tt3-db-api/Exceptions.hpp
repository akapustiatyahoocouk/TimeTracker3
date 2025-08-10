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
    class TT3_DB_API_PUBLIC DatabaseException : public QException
    {
        //////////
        //  Construction/destruction/assignment
    public:
        DatabaseException() : _errorMessage("Unknowqn database error") {}
        explicit DatabaseException(const QString & errorMessage) : _errorMessage(errorMessage) {}

        //////////
        //  QException
    public:
        virtual QException *    clone() const { return new DatabaseException(*this); }
        virtual void            raise() const { throw *this; }

        //////////
        //  Operations
    public:
        //  TODO document
        QString         errorMessage() const { return _errorMessage; }

        //////////
        //  Implementation
    private:
        QString         _errorMessage;
    };

    //  Thrown when an invalid database address is specified
    class TT3_DB_API_PUBLIC InvalidDatabaseAddressException : public DatabaseException
    {
        //////////
        //  Construction/destruction/assignment
    public:
        InvalidDatabaseAddressException() : DatabaseException("Invalid database address") {}

        //////////
        //  QException
    public:
        virtual QException *    clone() const { return new InvalidDatabaseAddressException(*this); }
        virtual void            raise() const { throw *this; }
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
        virtual QException *    clone() const { return new DatabaseInUseException(*this); }
        virtual void            raise() const { throw *this; }
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
        virtual QException *    clone() const { return new DatabaseCorruptException(*this); }
        virtual void            raise() const { throw *this; }
    };

    //  Thrown when a service is called on a closed database.
    class TT3_DB_API_PUBLIC DatabaseClosedException : public DatabaseException
    {
        //////////
        //  Construction/destruction/assignment
    public:
        explicit DatabaseClosedException(IDatabaseAddress * address)
            :   DatabaseException(address->databaseType()->displayName() + " "  +
                                  address->displayForm() + " is already in use") {}

        //////////
        //  QException
    public:
        virtual QException *    clone() const { return new DatabaseClosedException(*this); }
        virtual void            raise() const { throw *this; }
    };
}

//  End of tt3-db-api/Exceptions.hpp
