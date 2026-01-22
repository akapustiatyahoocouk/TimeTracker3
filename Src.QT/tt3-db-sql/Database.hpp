//
//  tt3-db-sql/Database.hpp - "SQL database"
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

namespace tt3::db::sql
{
    /// \class Database tt3-db-sql/API.hpp
    /// \brief A generia SQL database.
    class TT3_DB_SQL_PUBLIC Database
        :   public virtual tt3::db::api::IDatabase
    {
        TT3_CANNOT_ASSIGN_OR_COPY_CONSTRUCT(Database)

        friend class DatabaseType;

        //////////
        //  Construction/destruction
    protected:
        Database() = default;
    public:
        virtual ~Database() = default;

        //////////
        //  tt3::db::api::IDatabase (associations)
    public:
        virtual quint64 objectCount(
                            ) const override;
        virtual auto    findObjectByOid(
                                const tt3::db::api::Oid & oid
                            ) const -> tt3::db::api::IObject * override;
        virtual auto    users(
                            ) const -> tt3::db::api::Users override;
        virtual auto    accounts(
                            ) const -> tt3::db::api::Accounts override;
        virtual auto    findAccount(
                                const QString & login
                            ) const -> tt3::db::api::IAccount * override;
        virtual auto    activityTypes(
                            ) const -> tt3::db::api::ActivityTypes override;
        virtual auto    findActivityType(
                                const QString & displayName
                            ) -> tt3::db::api::IActivityType * override;
        virtual auto    publicActivities(
                            ) const -> tt3::db::api::PublicActivities override;
        virtual auto    findPublicActivity(
                                const QString & displayName
                            ) -> tt3::db::api::IPublicActivity * override;
        virtual auto    publicActivitiesAndTasks(
                            ) const -> tt3::db::api::PublicActivities override;
        virtual auto    publicTasks(
                            ) const -> tt3::db::api::PublicTasks override;
        virtual auto    rootPublicTasks(
                            ) const -> tt3::db::api::PublicTasks override;
        virtual auto    projects(
                            ) const -> tt3::db::api::Projects override;
        virtual auto    rootProjects(
                            ) const -> tt3::db::api::Projects override;
        virtual auto    workStreams(
                            ) const -> tt3::db::api::WorkStreams override;
        virtual auto    beneficiaries(
                            ) const -> tt3::db::api::Beneficiaries override;

        //////////
        //  tt3::db::api::IDatabase (access control)
    public:
        virtual auto    tryLogin(
                                const QString & login,
                                const QString & password
                            ) const -> tt3::db::api::IAccount * override;
        virtual auto    login(
                                const QString & login,
                                const QString & password
                            ) const -> tt3::db::api::IAccount * override;

        //////////
        //  tt3::db::api::IDatabase (life cycle)
    public:
        virtual auto    createUser(
                                bool enabled,
                                const QStringList & emailAddresses,
                                const QString & realName,
                                const tt3::db::api::InactivityTimeout & inactivityTimeout,
                                const tt3::db::api::UiLocale & uiLocale,
                                const tt3::db::api::Workloads & permittedWorkloads
                            ) -> tt3::db::api::IUser * override;
        virtual auto    createActivityType(
                                const QString & displayName,
                                const QString & description
                            ) -> tt3::db::api::IActivityType * override;
        virtual auto    createPublicActivity(
                                const QString & displayName,
                                const QString & description,
                                const tt3::db::api::InactivityTimeout & timeout,
                                bool requireCommentOnStart,
                                bool requireCommentOnStop,
                                bool fullScreenReminder,
                                tt3::db::api::IActivityType * activityType,
                                tt3::db::api::IWorkload * workload
                            ) -> tt3::db::api::IPublicActivity * override;
        virtual auto    createPublicTask(
                                const QString & displayName,
                                const QString & description,
                                const tt3::db::api::InactivityTimeout & timeout,
                                bool requireCommentOnStart,
                                bool requireCommentOnStop,
                                bool fullScreenReminder,
                                tt3::db::api::IActivityType * activityType,
                                tt3::db::api::IWorkload * workload,
                                bool completed,
                                bool requireCommentOnCompletion
                            ) -> tt3::db::api::IPublicTask * override;
        virtual auto    createProject(
                                const QString & displayName,
                                const QString & description,
                                const tt3::db::api::Beneficiaries & beneficiaries,
                                bool completed
                            ) -> tt3::db::api::IProject * override;
        virtual auto    createWorkStream(
                                const QString & displayName,
                                const QString & description,
                                const tt3::db::api::Beneficiaries & beneficiaries
                            ) -> tt3::db::api::IWorkStream * override;
        virtual auto    createBeneficiary(
                                const QString & displayName,
                                const QString & description,
                                const tt3::db::api::Workloads & workloads
                            ) -> tt3::db::api::IBeneficiary * override;

        //////////
        //  IDatabase (locking)
    public:
        virtual auto    lock(tt3::db::api::IDatabaseLock::LockType lockType,
                             unsigned long timeoutMs = ULONG_MAX
                            ) -> tt3::db::api::IDatabaseLock * override;

        //////////
        //  tt3::db::api::IDatabase (change notification handling)
    public:
        virtual auto    changeNotifier(
            ) -> tt3::db::api::ChangeNotifier * override
        {
            return &_changeNotifier;
        }

        //////////
        //  Implementation
    private:
        tt3::db::api::ChangeNotifier    _changeNotifier;
    };
}

//  End of tt3-db-sqlite3/Database.hpp
