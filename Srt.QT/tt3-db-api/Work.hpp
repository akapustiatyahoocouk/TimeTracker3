//
//  tt3-db-api/Work.hpp - a unit of work
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
    //  A unit of work
    class TT3_DB_API_PUBLIC IWork : public virtual IObject
    {
        //////////
        //  This is an interface
    protected:
        IWork() = default;
        virtual ~IWork() = default;

        //////////
        //  IObject
    public:
        virtual auto    type(
                            ) const
                            -> IObjectType * override
        {
            return ObjectTypes::Work::instance();
        }

        //////////
        //  Operations (properties)
    public:
        //  Returns the UTC date+time when this unit of work
        //  was started/finished.
        //  Throws DatabaseException if an error occurs.
        virtual auto    startedAt(
                            ) const
                            throws(DatabaseException)
                            -> QDateTime = 0;
        virtual auto    finishedAt(
                            ) const
                            throws(DatabaseException)
                            -> QDateTime = 0;

        //////////
        //  Operations (associations)
    public:
        //  Returns the User who has logged this unit of work.
        //  Throws DatabaseException if an error occurs.
        virtual auto    user(
                            ) const
                            throws(DatabaseException)
                            -> IUser * = 0;

        //  Returns the Activity against which this unit of work was logged.
        //  Throws DatabaseException if an error occurs.
        virtual auto    activity(
                            ) const
                            throws(DatabaseException)
                            -> IActivity * = 0;
    };
}

//  End of tt3-db-api/Work.hpp
