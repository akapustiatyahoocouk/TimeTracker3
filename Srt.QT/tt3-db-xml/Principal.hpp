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
        Principal(Database * database, tt3::db::api::Oid oid);
        virtual ~Principal();

        //////////
        //  tt3::db::api::IPrincipal (general)
    public:
        virtual bool    enabled(
                            ) const override;
        virtual void    setEnabled(
                                bool enabled
                            ) override;
        virtual auto    emailAddresses(
                            ) const -> QStringList override;
        virtual void    setEmailAddresses(
                                const QStringList & emailAddresses
                            ) override;

        //////////
        //  Implementation
    private:
        bool            _enabled = true;
        QStringList     _emailAddresses {};

        //  Helpers
        virtual void    _markDead() override;

        //////////
        //  Serialization
    private:
        virtual void    _serializeProperties(
                                QDomElement & objectElement
                            ) override;
        virtual void    _serializeAggregations(
                                QDomElement & parentElement
                            ) override;

        virtual void    _deserializeProperties(
                                const QDomElement & objectElement
                            ) override; //  throws tt3::util::ParseException) overrid
        virtual void    _deserializeAggregations(
                                const QDomElement & parentElement
                            ) override; //  throws tt3::util::ParseException) overrid

        //////////
        //  Validation
    private:
        virtual void    _validate(
                                Objects & validatedObjects
                            ) override; //  throws tt3::db::api::DatabaseException) overrid
    };
}

//  End of tt3-db-xml/Principal.hpp
