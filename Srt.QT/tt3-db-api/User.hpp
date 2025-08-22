//
//  tt3-db-api/User.hpp - a user
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
    //  A user
    class TT3_DB_API_PUBLIC IUser : public virtual IPrincipal
    {
        //////////
        //  This is an interface
    protected:
        IUser() = default;
        virtual ~IUser() = default;

        //////////
        //  IObject
    public:
        virtual IObjectType *   type() const override
        {
            return ObjectTypes::User::instance();
        }

        //////////
        //  Operations (properties)
    public:
        //  Returns the "real name" of this User.
        //  Throws DatabaseException if an error occurs.
        virtual QString     realName() const throws(DatabaseException) = 0;

        //  Sets the "real name" of this User.
        //  Throws DatabaseException if an error occurs.
        virtual void        setRealName(const QString & realName) throws(DatabaseException) = 0;

        //  Returns the "inactivity timeout" of this User.
        //  This is the timeout after which, if the user does nothing,
        //  the "current" Activity is automatically stopped.
        //  Throws DatabaseException if an error occurs.
        virtual std::optional<tt3::util::TimeSpan>
                            inactivityTimeout() const throws(DatabaseException) = 0;

        //  Sets the "inactivity timeout" of this User.
        //  This is the timeout after which, if the user does nothing,
        //  the "current" Activity is automatically stopped.
        //  Throws DatabaseException if an error occurs.
        virtual void        setInactivityTimeout(const std::optional<tt3::util::TimeSpan> & inactivityTimeout) throws(DatabaseException) = 0;

        //  Returns the preferred UI locale  of this User.
        //  Throws DatabaseException if an error occurs.
        virtual std::optional<QLocale>
                            uiLocale() const throws(DatabaseException) = 0;

        //  Sets the preferred UI locale  of this User.
        //  Throws DatabaseException if an error occurs.
        virtual void        setUiLocale(const std::optional<QLocale> & uiLocale) throws(DatabaseException) = 0;

        //////////
        //  Operations (associations)
    public:
        //  Returns the set of all Accounts of this User.
        //  Throws DatabaseException if an error occurs.
        virtual Accounts    accounts() const throws(DatabaseException) = 0;

        //////////
        //  Operations (life cycle)
    public:
        //  Creates a new Account for this User.
        //  All Accounts in a database must have different lohins.
        //  Throws DatabaseException if an error occurs.
        virtual IAccount *  createAccount(
            bool enabled, const QStringList & emailAddresses,
            const QString & login, const QString & password,
            Capabilities capabilities) throws(DatabaseException) = 0;
    };
}

//  End of tt3-db-api/User.hpp
