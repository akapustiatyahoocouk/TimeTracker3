//
//  tt3-db-api/Account.hpp - a login account
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
    //  A login account
    class TT3_DB_API_PUBLIC IAccount :
        public virtual IPrincipal
    {
        //////////
        //  This is an interface
    protected:
        IAccount() = default;
        virtual ~IAccount() = default;

        //////////
        //  IObject
    public:
        virtual auto    type() const
                            -> IObjectType * override
        {
            return ObjectTypes::Account::instance();
        }

        //////////
        //  Operations (properties)
    public:
        //  Returns/sets the login identifier of this account.
        //  Throws DatabaseException if an error occurs.
        virtual auto    login() const
                            throws(DatabaseException)
                            -> QString = 0;
        virtual void    setLogin(
                                const QString & login
                            )
                            throws(DatabaseException) = 0;

        //  Returns the SHA-1 hash of rhis account's password
        //  as an uppercase hexstring.
        //  Throws DatabaseException if an error occurs.
        virtual auto    passwordHash(
                            ) const
                            throws(DatabaseException)
                            -> QString = 0;

        //  Sets the password of this account.
        //  Throws DatabaseException if an error occurs.
        virtual void    setPassword(
                                const QString & password
                            )
                            throws(DatabaseException) = 0;

        //  Returns/sets the capabilities of this account.
        //  Throws DatabaseException if an error occurs.
        virtual auto    capabilities(
                            ) const
                            throws(DatabaseException)
                            -> Capabilities = 0;
        virtual void    setCapabilities(
                                Capabilities capabilities
                            )
                            throws(DatabaseException) = 0;

        //////////
        //  Operations (associations)
    public:
        //  Returns the user to whom this account belongs.
        //  Throws DatabaseException if an error occurs.
        virtual auto    user() const
                            throws(DatabaseException)
                            -> IUser * = 0;

        //  Returns/sets the lick of "quick pick" activities
        //  associated with this account.
        //  Throws DatabaseException if an error occurs.
        virtual auto    quickPickList(
                            ) const throws(DatabaseException)
                            -> QList<IActivity*> = 0;
        virtual void    setQuickPickList(
                                const QList<IActivity*> & quickPickList
                            ) const
                            throws(DatabaseException) = 0;
    };
}

//  End of tt3-db-api/Account.hpp
