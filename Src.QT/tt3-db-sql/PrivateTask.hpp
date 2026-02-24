//
//  tt3-db-sql/PrivateTask.hpp - a private task
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
    /// \class PrivateTask tt3-db-sql/API.hpp
    /// \brief A private task in an SQL database.
    class TT3_DB_SQL_PUBLIC PrivateTask final
        :   public PrivateActivity,
            public Task,
            public virtual tt3::db::api::IPrivateTask
    {
        TT3_CANNOT_ASSIGN_OR_COPY_CONSTRUCT(PrivateTask)

        friend class Database;
        friend class User;

        //////////
        //  Construction/destruction (from DB type only)
    private:
        PrivateTask(Database * database, qint64 pk);
        virtual ~PrivateTask();

        //////////
        //  tt3::db::api::IPrivateTask (associations)
    public:
        virtual auto    parent(
                            ) const -> tt3::db::api::IPrivateTask * override;
        virtual void    setParent(
                                tt3::db::api::IPrivateTask * parent
                            ) override;
        virtual auto    children(
                            ) const -> tt3::db::api::PrivateTasks override;

        //////////
        //  tt3::db::api::IPrivateTask (life cycle)
    public:
        virtual auto    createChild(
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
                            ) -> tt3::db::api::IPrivateTask * override;

        //////////
        //  Implementation
    private:
        //  Cached properties
        virtual void    _invalidateCachedProperties() override;
        virtual void    _loadCachedProperties() override;

        //  Helpers
        virtual void    _deleteCascade() override;  //  may throw
        virtual bool    _siblingExists(const QString & displayName) const override;
        PrivateTask *   _findChild(const QString & displayName) const;
        void            _collectParentClosure(PrivateTasks & closure);
    };
}

//  End of tt3-db-sql/PublicTask.hpp
