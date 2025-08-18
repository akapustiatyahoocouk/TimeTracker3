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
        virtual bool                isOpen() const = 0;
        virtual void                close() throws(DatabaseException) = 0;

        //////////
        //  Operations (associations)
    public:
        //  The set of all users in this database
        virtual Users       users() const throws(DatabaseException) = 0;

        //  The set of all accounts of all users in this database
        virtual Accounts    accounts() const throws(DatabaseException) = 0;

        //  The account with the specified login
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
        //  TODO document
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
