//
//  tt3-db-xml/DatabaseLock.hpp - "XML file database LOCK" ADT
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
#pragma once
#include "tt3-db-xml/API.hpp"

namespace tt3::db::xml
{
    /// \class DatabaseLock tt3-db-xml/API.hpp
    /// \brief The "XML file database lock".
    class TT3_DB_XML_PUBLIC DatabaseLock final
        :   public QObject,
            public virtual tt3::db::api::IDatabaseLock
    {
        CANNOT_ASSIGN_OR_COPY_CONSTRUCT(DatabaseLock)

        friend class Database;

        //////////
        //  Construction/destruction
    private:    //  construction is from friends only
        DatabaseLock(
                Database * database,
                LockType lockType,
                QThread * lockingThread
            );
    public:
        virtual ~DatabaseLock();

        //////////
        //  IDatabaseLock
    public:
        virtual LockType    lockType() const override { return _lockType; }

        //////////
        //  Implementation
    private:
        Database *      _database;  //  nullptr == lock is an orphan
        const LockType  _lockType;
        QThread *       _lockingThread; //  nullptr for orphan locks
    };
}

//  End of tt3-db-xml/DatabaseLock.hpp
