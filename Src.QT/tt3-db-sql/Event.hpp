//
//  tt3-db-sql/Event.hpp - an event
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
    /// \class Event tt3-db-sql/API.hpp
    /// \brief An event in an SQL database.
    class TT3_DB_SQL_PUBLIC Event final
        :   public Object,
            public virtual tt3::db::api::IEvent
    {
        TT3_CANNOT_ASSIGN_OR_COPY_CONSTRUCT(Event)

        friend class Database;
        friend class Account;

        //////////
        //  Construction/destruction (from DB type only)
    private:
        Event(Database * database, qint64 pk);
        virtual ~Event();

        //////////
        //  tt3::db::api::IEvent (properties)
    public:
        virtual auto    occurredAt(
                            ) const -> QDateTime override;
        virtual auto    summary(
                            ) const -> QString override;

        //////////
        //  tt3::db::api::IEvent (associations)
    public:
        virtual auto    account(
                            ) const -> tt3::db::api::IAccount * override;
        virtual auto    activities(
                            ) const -> tt3::db::api::Activities override;

        //////////
        //  Implementation
    private:
        //  Cached properties
        CachedProperty<QDateTime>   _occurredAt;
        CachedProperty<QString>     _summary;

        CachedProperty<qint64>      _fkAccount;
        CachedProperty<QSet<qint64>>_fkActivities;

        virtual void    _invalidateCachedProperties() override;
        virtual void    _loadCachedProperties() override;

        //  Helpers
        virtual void    _deleteCascade() override;  //  may throw
        virtual void    _removeFromDatabase() override; //  may throw
        virtual QString _tableName() const override { return "events"; }
    };
}

//  End of tt3-db-sql/Event.hpp
