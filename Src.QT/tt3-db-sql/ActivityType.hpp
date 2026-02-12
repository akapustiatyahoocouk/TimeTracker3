//
//  tt3-db-sql/ActivityType.hpp - an activity type
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
    /// \class ActivityType tt3-db-sql/API.hpp
    /// \brief An activity type in an SQL database.
    class TT3_DB_SQL_PUBLIC ActivityType final
        :   public Object,
            public virtual tt3::db::api::IActivityType
    {
        TT3_CANNOT_ASSIGN_OR_COPY_CONSTRUCT(ActivityType)

        friend class Database;

        //////////
        //  Construction/destruction (from DB type only)
    private:
        ActivityType(Database * database, qint64 pk);
        virtual ~ActivityType();

        //////////
        //  tt3::db::api::IActivityType (properties)
    public:
        virtual QString displayName(
                            ) const override;
        virtual void    setDisplayName(
                                const QString & displayName
                            ) override;
        virtual QString description(
                            ) const override;
        virtual void    setDescription(
                                const QString & description
                            ) override;

        //////////
        //  tt3::db::api::IActivityType (associations)
    public:
        virtual auto    activities(
                            ) const -> tt3::db::api::Activities override;

        //////////
        //  Implementation
    private:
        //  Cached properties
        CachedProperty<QString> _displayName;
        CachedProperty<QString> _description;

        virtual void    _invalidateCachedProperties() override;
        virtual void    _loadCachedProperties() override;
        void            _saveDisplayName(const QString & displayName);
        void            _saveDescription(const QString & description);

        //  Helpers
        virtual void    _deleteCascade() override;  //  may throw
        virtual void    _removeFromDatabase() override; //  may throw
        virtual QString _tableName() const override { return "activitytypes"; }
    };
}

//  End of tt3-db-sql/ActivityType.hpp
