//
//  tt3-db-sql/PrivateActivity.hpp - a Private activity
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
    /// \class PrivateActivity tt3-db-xml/API.hpp
    /// \brief A private activity in an SQL database.
    class TT3_DB_SQL_PUBLIC PrivateActivity
        :   public virtual Activity,
            public virtual tt3::db::api::IPrivateActivity
    {
        TT3_CANNOT_ASSIGN_OR_COPY_CONSTRUCT(PrivateActivity)

        friend class Database;
        friend class PrivateTask;

        //////////
        //  Construction/destruction (from DB type only)
    private:
        PrivateActivity(Database * database, qint64 pk);
        virtual ~PrivateActivity();

        //////////
        //  tt3::db::api::IPrivateActivity (associations)
    public:
        virtual auto    owner(
                            ) const -> tt3::db::api::IUser * override;

        //////////
        //  Implementation
    private:
        //  Cached properties
        CachedProperty<qint64>  _fkOwner;

        virtual void    _invalidateCachedProperties() override;
        virtual void    _loadCachedProperties() override;

        //  Helpers
        virtual bool    _siblingExists(const QString & displayName) const override;
    };
}

//  End of tt3-db-sql/PrivateActivity.hpp
