//
//  tt3-db-xml/Principal.hpp - a generic access control principal
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

namespace tt3::db::xml
{
    //////////
    //  A generic access control principal
    class TT3_DB_XML_PUBLIC Principal
        :   public Object,
            public virtual tt3::db::api::IPrincipal
    {
        CANNOT_ASSIGN_OR_COPY_CONSTRUCT(Principal)

        friend class Database;
        friend class User;
        friend class Account;

        //////////
        //  Construction/destruction (from DB type only)
    private:
        Principal(Database * database, Oid oid);
        virtual ~Principal();

        //////////
        //  tt3::db::api::IPrincipal (general)
    public:
        virtual bool        enabled() const throws(DatabaseException) override;
        virtual void        setEnabled(bool enabled) throws(DatabaseException) override;
        virtual QStringList emailAddresses() const throws(DatabaseException) override;
        virtual void        setEmailAddresses(const QStringList & emailAddresses) throws(DatabaseException) override;

        //////////
        //  Implementation
    private:
        bool            _enabled = true;
        QStringList     _emailAddresses {};

        //////////
        //  Serialization
    private:
        virtual void        _serializeProperties(QDomElement & objectElement) override;
        virtual void        _serializeAggregations(QDomElement & parentElement);
    };
}

//  End of tt3-db-api/Principal.hpp
