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
        //  Returns the type of this database.
        //  Can be safely obtained on both open and closed databases.
        virtual IDatabaseType *     type() const = 0;

        //  Returns the address of this database.
        //  Can be safely obtained on both open and closed databases.
        virtual IDatabaseAddress *  address() const = 0;

        //  Returns the validator used by this database.
        virtual IValidator *        validator() const = 0;

        //  Checks whether this database is open or closed.
        //  All database instances start off as open.
        virtual bool                isOpen() const = 0;

        //  Closes this database; jas no effect if already closed.
        //  Throws DatabaseException if an error occurs.
        virtual void                close() throws(DatabaseException) = 0;

        //////////
        //  Operations (associations)
    public:
        //  The set of all users in this database.
        //  Throws DatabaseException if an error occurs.
        virtual Users       users() const throws(DatabaseException) = 0;

        //  The set of all accounts of all users in this database.
        //  Throws DatabaseException if an error occurs.
        virtual Accounts    accounts() const throws(DatabaseException) = 0;

        //  The account with the specified login.
        //  Throws DatabaseException if an error occurs.
        virtual IAccount *  findAccount(const QString & login) const throws(DatabaseException) = 0;

        //////////
        //  Operations (access control)
    public:
        //  If there exists an a) enabled account b) of an
        //  enabled user c) with the spcified login and
        //  password, returns it; otherwise returns nullptr.
        virtual IAccount *  tryLogin(const QString & login, const QString & password) const throws(DatabaseException) = 0;

        //  If there exists an a) enabled account b) of an
        //  enabled user c) with the spcified login and
        //  password, returns it; otherwise throws an
        //  AccessDeniedException.
        virtual IAccount *  login(const QString & login, const QString & password) const throws(DatabaseException);

        //////////
        //  Opertions (life cycle)
    public:
        //  Creates a new User in this database.
        //  Throws DatabaseException if an error occurs.
        virtual IUser *     createUser(bool enabled, const QStringList & emailAddresses,
                                const QString & realName,
                                const std::optional<tt3::util::TimeSpan> & inactivityTimeout,
                                const std::optional<QLocale> & uiLocale) throws(DatabaseException) = 0;

        //////////
        //  Operations (change notification handling)
    public:
        //  The change notifier used by this database
        virtual ChangeNotifier &changeNotifier() = 0;
    };
}

//  End of tt3-db-api/Database.hpp
