//
//  tt3-db-xml/Account.hpp - a login account
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
    //  A login account
    class TT3_DB_XML_PUBLIC Account final
        :   public Principal,
            public virtual tt3::db::api::IAccount
    {
        CANNOT_ASSIGN_OR_COPY_CONSTRUCT(Account)

        friend class Database;
        friend class User;

        //////////
        //  Construction/destruction (from DB type only)
    private:
        Account(User * user, Oid oid);
        virtual ~Account();

        //////////
        //  tt3::db::api::IObject (life cycle)
    public:
        virtual void        destroy() throws(DatabaseException) override;

        //////////
        //  tt3::db::api::IAccount (properties)
    public:
        virtual QString     login() const throws(DatabaseException) override;
        virtual void        setLogin(const QString & login) throws(DatabaseException) override;
        virtual QString     passwordHash() const throws(DatabaseException) override;
        virtual void        setPassword(const QString & password) throws(DatabaseException) override;
        virtual tt3::db::api::Capabilities  capabilities() const throws(DatabaseException) override;
        virtual void        setCapabilities(tt3::db::api::Capabilities capabilities) throws(DatabaseException) override;

        //////////
        //  tt3::db::api::IAccount (associations)
    public:
        virtual tt3::db::api::IUser *   user() const throws(DatabaseException) override;

        //////////
        //  Implementation
    private:
        //  Properties
        QString             _login;
        QString             _passwordHash;  //  SHA-1 uppercase hexstring
        tt3::db::api::Capabilities  _capabilities;
        //  Associations
        User *              _user;  //  counts as "reference"

        //////////
        //  Serialization
    private:
        virtual void        _serializeProperties(QDomElement & objectElement) override;
        virtual void        _serializeAggregations(QDomElement & parentElement) override;
    };
}

//  End of tt3-db-api/User.hpp
