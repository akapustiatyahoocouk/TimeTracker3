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
        friend class Statement;
        friend class Object;
        friend class Principal;
        friend class User;

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
        //  Operations (database specifics)
    protected:
        /// \brief
        ///     Checks if an identifier can start with the
        ///     specified character.
        /// \details
        ///     The default implementation allows a-z, A-Z or _,
        ///     as per SQL standard.
        /// \param c
        ///     The character to check.
        /// \return
        ///     True if an identifier can start with the
        ///     specified character, else false.
        virtual bool    isIdentifierStart(const QChar & c) const;

        /// \brief
        ///     Checks if an identifier can contain the
        ///     specified character.
        /// \details
        ///     The default implementation allows a-z, A-Z, 0-9
        ///     or _, as per SQL standard.
        /// \param c
        ///     The character to check.
        /// \return
        ///     True if an identifier can contain the
        ///     specified character, else false.
        virtual bool    isIdentifierChar(const QChar & c) const;

        /// \brief
        ///     Checks if the specified word is a valid identifier.
        /// \param word
        ///     The work to check.
        /// \return
        ///     True if the specified word is a valid identifier, else false.
        virtual bool    isIdentifier(const QString & word) const;

        /// \brief
        ///     Checks if the specified work is a SQL keyword.
        /// \details
        ///     The set of keywprds is specific to SQL database.
        /// \param word
        ///     The word to check (ignoring case).
        /// \return
        ///     True if the specified work is a SQL keyword, else false.
        virtual bool    isKeyword(const QString & word) const = 0;

        /// \brief
        ///     Quotes the identifier if necessary.
        /// \details
        ///     The syntax of quoted identifiers is specific to SQL database.
        ///     he identifier upper/lower casing is preserved.
        /// \param identifier
        ///     The identifier.
        /// \return
        ///     The identifier, quoted if necessary, "as is" if not.
        virtual QString quoteIdentifier(const QString & identifier) const = 0;

        /// \brief
        ///     Executes the SQL script.
        /// \details
        ///     The current transaction context (absent/present) is preserved.
        /// \param sql
        ///     The SQL script. Statement are separated by ';', comments
        ///     and empty statements are ignored.
        /// \exception DatabaseException
        ///     If an error occurs. Wrap the script execution into a
        ///     transaction to perform clean rollback if this happens.
        virtual void    executeScript(const QString & sql);

        /// \brief
        ///     Creates a new SQL statement from the specified SQL template.
        /// \details
        ///     The caller is responsible for deleteing it when done.
        /// \param sqlTemplate
        ///     The SQL statement template with ? placeholders.
        /// \return
        ///     The newly created SQL statement.
        /// \exception DatabaseException
        ///     If an error occurs.
        virtual auto    createStatement(const QString & sqlTemplate) -> Statement *;

        //  TODO document
        virtual qint64  executeInsert(const QString & sql) = 0;
        virtual qint64  executeUpdate(const QString & sql) = 0;
        virtual qint64  executeDelete(const QString & sql) = 0;
        virtual auto    executeSelect(const QString & sql) -> ResultSet * = 0;
        virtual void    execute(const QString & sql) = 0;

        /// \brief
        ///     Checks if the database is open.
        /// \details
        ///     The implementation can safely assime the
        ///     "guard" is already locked by the calling thread.
        /// \exception DatabaseException
        ///     If the database is closed.
        virtual void    ensureOpen() const = 0;

        /// \brief
        ///     Checks if the database is open and writable.
        /// \details
        ///     The implementation can safely assime the
        ///     "guard" is already locked by the calling thread.
        /// \exception DatabaseException
        ///     If the database is closed or read-only.
        virtual void    ensureOpenAndWritable() const = 0;

        //////////
        //  Implementation
    protected:
        /// \brief
        ///     The guard for all access synchronization.
        mutable tt3::util::Mutex    guard;
    private:
        tt3::db::api::ChangeNotifier    _changeNotifier;

        //  Object caches - NOT count as "references"
        QMap<qint64, Object*>   _liveObjects;   //  All "live" objects
        QMap<qint64, Object*>   _graveyard;     //  All "dead" objects
    };
}

//  End of tt3-db-sqlite3/Database.hpp
