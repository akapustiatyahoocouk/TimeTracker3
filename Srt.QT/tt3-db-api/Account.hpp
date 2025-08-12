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
    //  Capabilities of a login account
    enum class Capabilities
    {
        Administrator = 0x0001,
        ManageUsers = 0x0002,
        ManageActivityTypes = 0x0004,
        ManageBeheficiaries = 0x0008,
        ManageWorkloads = 0x0010,
        ManagePublicActivities = 0x0020,
        ManagePublicTasks = 0x0040,
        ManagePrivateActivities = 0x0080,
        ManagePrivateTasks = 0x0100,
        LogWork = 0x0200,
        LogEvents = 0x0400,
        GenerateReports = 0x0800,
        BackupAndRestore = 0x1000
    };

    //  A login account
    class TT3_DB_API_PUBLIC IAccount : public virtual IPrincipal
    {
        //////////
        //  This is an interface
    protected:
        IAccount() = default;
        virtual ~IAccount() = default;

        //////////
        //  IDatabaseObject
    public:
        virtual IDatabaseObjectType *   type() const override
        {
            return DatabaseObjectTypes::Account::instance();
        }

        //////////
        //  Operations (properties)
    public:
        //  TODO document
        virtual QString         login() const throws(DatabaseException) = 0;
        virtual void            setLogin(const QString & login) throws(DatabaseException) = 0;
        virtual QString         passwordHash() const throws(DatabaseException) = 0;
        virtual void            setPassword(const QString & password) throws(DatabaseException) = 0;
        virtual Capabilities    capabilities() const throws(DatabaseException) = 0;
        virtual void            setCapabilities(Capabilities capabilities) throws(DatabaseException) = 0;

        //////////
        //  Operations (associations)
    public:
        //  TODO document
        virtual IUser *         user() const throws(DatabaseException) = 0;
    };
}

//  End of tt3-db-api/Account.hpp
