//
//  tt3-db-sql/WorkStream.hpp - a work stream
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
    /// \class WorkStream tt3-db-sql/API.hpp
    /// \brief A work stream in an SQL database.
    class TT3_DB_SQL_PUBLIC WorkStream final
        :   public Workload,
            public virtual tt3::db::api::IWorkStream
    {
        TT3_CANNOT_ASSIGN_OR_COPY_CONSTRUCT(WorkStream)

        friend class Database;

        //////////
        //  Construction/destruction (from DB type only)
    private:
        WorkStream(Database * database, qint64 pk);
        virtual ~WorkStream();

        //////////
        //  Implementation
    private:
        //  Cached properties
        virtual void    _loadCachedProperties() override;

        //  Helpers
        virtual bool    _siblingExists(const QString & displayName) const override;
    };
}

//  End of tt3-db-sql/WorkStream.hpp
