//
//  tt3-db-api/Event.hpp - a recorded evemt
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
    //  A recorded event
    class TT3_DB_API_PUBLIC IEvent : public virtual IObject
    {
        //////////
        //  This is an interface
    protected:
        IEvent() = default;
        virtual ~IEvent() = default;

        //////////
        //  IObject
    public:
        virtual IObjectType *   type() const override
        {
            return ObjectTypes::Event::instance();
        }

        //////////
        //  Operations (properties)
    public:
        //  Returns the UTC date+time when this event was logged.
        //  Throws DatabaseException if an error occurs.
        virtual QDateTime       occurredAt() const throws(DatabaseException) = 0;

        //  The summary of this event, as entered by the user.
        //  Throws DatabaseException if an error occurs.
        virtual QString         summary() const throws(DatabaseException) = 0;

        //////////
        //  Operations (associations)
    public:
        //  Returns the User who has logged this event.
        //  Throws DatabaseException if an error occurs.
        virtual IUser *         user() const throws(DatabaseException) = 0;

        //  Returns the Activity against which this event was logged;
        //  nullptr if none.
        //  Throws DatabaseException if an error occurs.
        virtual IActivity *     activity() const throws(DatabaseException) = 0;
    };
}

//  End of tt3-db-api/Work.hpp
