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
    class TT3_DB_API_PUBLIC IPrincipal : public virtual IObject
    {
        //////////
        //  This is an interface
    protected:
        IPrincipal() = default;
        virtual ~IPrincipal() = default;

        //////////
        //  Operations (properties)
    public:
        //  Checks/sets whether this Principal is "enabled". "Disabled"
        //  Principals do not exist as far as access control is concerned.
        //  Throws DatabaseException if an error occurs.
        virtual bool    enabled(
                            ) const
                            throws(DatabaseException) = 0;
        virtual void    setEnabled(
                                bool enabled
                            )
                            throws(DatabaseException) = 0;

        //  Returns/sets the list of e-mail addresses associated with
        //  this Principal.
        //  Throws DatabaseException if an error occurs.
        virtual auto    emailAddresses(
                            ) const
                            throws(DatabaseException)
                            -> QStringList = 0;
        virtual void    setEmailAddresses(
                                const QStringList & emailAddresses
                            )
                            throws(DatabaseException) = 0;
    };
}

//  End of tt3-db-api/Principal.hpp
