//
//  tt3-db-api/DatabaseAddress.hpp - "database address" ADT
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

namespace db::api
{
    //////////
    //  A "database address" represents a location of a speific
    //  database of a given type. Database address instances are
    //  managed by their corresponding database type.
    class TT3_DB_API_PUBLIC IDatabaseAddress
    {
        //////////
        //  This is an interface
    public:
        IDatabaseAddress() = default;
    protected:
        virtual ~IDatabaseAddress() = default;

        //////////
        //  Operations
    public:
        //  The database type to which this database address belongs.
        virtual IDatabaseType * databaseType() const = 0;

        //  The user-readable form of this database address.
        virtual QString         displayForm() const = 0;

        //  The external (re-parsable) form of this database address.
        virtual QString         externalForm() const = 0;
    };
}

//  End of tt3-db-api/DatabaseAddress.hpp
