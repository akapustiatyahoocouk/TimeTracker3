//
//  tt3-db-sql/Task.hpp - a generic task
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
    /// \class Task tt3-db-sql/API.hpp
    /// \brief A generic task in an SQL database.
    class TT3_DB_SQL_PUBLIC Task
        :   public virtual Activity,
            public virtual tt3::db::api::ITask
    {
        TT3_CANNOT_ASSIGN_OR_COPY_CONSTRUCT(Task)

        friend class PublicTask;
        friend class PrivateTask;

        //////////
        //  Construction/destruction (from DB type only)
    private:
        Task(Database * database, qint64 pk);
        virtual ~Task();

        //////////
        //  tt3::db::api::ITask (properties)
    public:
        virtual bool    completed(
                            ) const override;
        virtual void    setCompleted(
                                bool completed
                            ) override;
        virtual bool    requireCommentOnCompletion(
                            ) const override;
        virtual void    setRequireCommentOnCompletion(
                                bool requireCommentOnCompletion
                            ) override;

        //////////
        //  Implementation
    private:
        //  Cached properties
        CachedProperty<bool>    _requireCommentOnCompletion;
        CachedProperty<bool>    _completed;

        CachedProperty<std::optional<qint64>>   _fkParent;

        virtual void    _invalidateCachedProperties() override;
        void            _saveRequireCommentOnCompletion(bool requireCommentOnCompletion);
        void            _saveCompleted(bool completed);
        void            _saveFkParent(const std::optional<qint64> & fkParent);
    };
}

//  End of tt3-db-sql/Task.hpp
