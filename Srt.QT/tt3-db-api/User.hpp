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
        //  IDatabaseObject
    public:
        virtual IDatabaseObjectType *   type() const override
        {
            return DatabaseObjectTypes::User::instance();
        }

        //////////
        //  Operations (properties)
    public:
        //  TODO document
        virtual QString     realName() const throws(DatabaseException) = 0;
        virtual void        setRealName(const QString & realName) throws(DatabaseException) = 0;
        virtual std::optional<tt3::util::TimeSpan>
                            inactivityTimeout() const throws(DatabaseException) = 0;
        virtual void        setInactivityTimeout(const std::optional<tt3::util::TimeSpan> & inactivityTimeout) throws(DatabaseException) = 0;
        virtual std::optional<QLocale>
                            uiLocale() const throws(DatabaseException) = 0;
        virtual void        setUiLocale(const std::optional<QLocale> & uiLocale) throws(DatabaseException) = 0;

        //////////
        //  Operations (associations)
    public:
        //  TODO document
        virtual Accounts    accounts() const throws(DatabaseException) = 0;
    };
}

//  End of tt3-db-api/User.hpp
