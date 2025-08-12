//
//  tt3-db-api/Principal.hpp - an access control principal
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
    //  A generic access control principal
    class TT3_DB_API_PUBLIC IPrincipal : public virtual IDatabaseObject
    {
        //////////
        //  This is an interface
    protected:
        IPrincipal() = default;
        virtual ~IPrincipal() = default;

        //////////
        //  Operations (properties)
    public:
        //  TODO document
        virtual bool        enabled() const throws(DatabaseException) = 0;
        virtual void        setEnabled(bool enabled) throws(DatabaseException) = 0;
        virtual QStringList emailAddresses() const throws(DatabaseException) = 0;
        virtual void        setEmailAddresses(const QStringList & emailAddresses) throws(DatabaseException) = 0;
    };
}

//  End of tt3-db-api/Principal.hpp
