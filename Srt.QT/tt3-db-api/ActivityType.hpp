//
//  tt3-db-api/ActivityType.hpp - an activity type
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
    //  An activity type
    class TT3_DB_API_PUBLIC IActivityType :
        public virtual IObject
    {
        //////////
        //  This is an interface
    protected:
        IActivityType() = default;
        virtual ~IActivityType() = default;

        //////////
        //  IObject
    public:
        virtual auto    type() const
                            -> IObjectType * override
        {
            return ObjectTypes::ActivityType::instance();
        }

        //////////
        //  Operations (properties)
    public:
        //  Returns/sets the user-readable display name of
        //  this activity type.
        //  Throws DatabaseException if an error occurs.
        virtual auto    displayName(
                            ) const
                            throws(DatabaseException)
                            -> QString = 0;
        virtual void    setDisplayName(
                                const QString & displayName
                            )
                            throws(DatabaseException) = 0;

        //  Returns/sets the multi-line description of this activity
        //  type, with lines separated by a newline character '\n'.
        //  Throws DatabaseException if an error occurs.
        virtual auto    description(
                            ) const
                            throws(DatabaseException)
                            -> QString = 0;
        virtual void    setDescription(
                                const QString & description
                            )
                            throws(DatabaseException) = 0;

        //////////
        //  Operations (associations)
    public:
        //  The set of all activities assigned to this activity type.
        //  Throws DatabaseException if an error occurs.
        virtual auto    activities(
                            ) const
                            throws(DatabaseException)
                            -> Activities = 0;
    };
}

//  End of tt3-db-api/ActivityType.hpp
