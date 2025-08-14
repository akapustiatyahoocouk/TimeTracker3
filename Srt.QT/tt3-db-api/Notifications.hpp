//
//  tt3-db-api/Notifications.hpp - database change notifications
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

namespace tt3::db::api
{
    //  Yje common base class for all database change notifications.
    class TT3_DB_API_PUBLIC ChangeNotification
    {
        //////////
        //  Construction/destruction
    protected:
        ChangeNotification(IDatabase * db)
            :   database(db) { Q_ASSERT(database != nullptr); }
        virtual ~ChangeNotification() = default;

        //////////
        //  Properties
    public:
        //  The database where the change has occurred
        IDatabase *const    database;
    };
}

//  End of tt3-db-api/Notifications.hpp
