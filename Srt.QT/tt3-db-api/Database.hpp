//
//  tt3-db-api/Database.hpp - "database" ADT
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
    //  A "database" represents an open connection
    //  to a persistent data container.
    class TT3_DB_API_PUBLIC IDatabase
    {
        //////////
        //  This is an interface
    public:
        IDatabase() = default;
        virtual ~IDatabase() = default;

        //////////
        //  Operations (general)
    public:
        //  TODO document
        virtual IDatabaseType *     type() const = 0;
        virtual IDatabaseAddress *  address() const = 0;
        virtual IValidator *        validator() const = 0;
        virtual bool                isOpen() = 0;
        virtual void                close() throws(DatabaseException) = 0;
    };
}

//  End of tt3-db-api/Database.hpp
