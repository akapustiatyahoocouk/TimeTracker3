//
//  tt3-db-sql/Work.hpp - a work unit
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
    /// \class Work tt3-db-sql/API.hpp
    /// \brief A work unit in an SQL database.
    class TT3_DB_SQL_PUBLIC Work final
        :   public Object,
            public virtual tt3::db::api::IWork
    {
        TT3_CANNOT_ASSIGN_OR_COPY_CONSTRUCT(Work)

        friend class Database;
        friend class Account;

        //////////
        //  Construction/destruction (from DB type only)
    private:
        Work(Database * database, qint64 pk);
        virtual ~Work();

        //////////
        //  tt3::db::api::IWork (properties)
    public:
        virtual auto    startedAt(
                            ) const -> QDateTime override;
        virtual auto    finishedAt(
                            ) const -> QDateTime override;

        //////////
        //  tt3::db::api::IWork (associations)
    public:
        virtual auto    account(
                            ) const -> tt3::db::api::IAccount * override;
        virtual auto    activity(
                            ) const -> tt3::db::api::IActivity * override;

        //////////
        //  Implementation
    private:
        //  Cached roperties
        CachedProperty<QDateTime>   _startedAt;
        CachedProperty<QDateTime>   _finishedAt;

        CachedProperty<qint64>     _fkAccount;
        CachedProperty<qint64>     _fkActivity;

        virtual void    _invalidateCachedProperties() override;
        virtual void    _loadCachedProperties() override;

        //  Helpers
        virtual void    _deleteCascade() override;  //  may throw
        virtual void    _removeFromDatabase() override; //  may throw
        virtual QString _tableName() const override { return "works"; }
    };
}

//  End of tt3-db-sql/Work.hpp
