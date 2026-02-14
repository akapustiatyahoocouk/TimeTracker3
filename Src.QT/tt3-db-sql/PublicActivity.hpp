//
//  tt3-db-sql/PublicActivity.hpp - a public activity
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
    /// \class PublicActivity tt3-db-xml/API.hpp
    /// \brief A public activity in an SQL database.
    class TT3_DB_SQL_PUBLIC PublicActivity
        :   public virtual Activity,
            public virtual tt3::db::api::IPublicActivity
    {
        TT3_CANNOT_ASSIGN_OR_COPY_CONSTRUCT(PublicActivity)

        friend class Database;
        friend class PublicTask;

        //////////
        //  Construction/destruction (from DB type only)
    private:
        PublicActivity(Database * database, qint64 pk);
        virtual ~PublicActivity();

        //////////
        //  Implementation
    private:
        //  Cached properties
        virtual void    _loadCachedProperties() override;

        //  Helpers
        virtual bool    _siblingExists(const QString & displayName) const override;
    };
}

//  End of tt3-db-sql/PublicActivity.hpp
