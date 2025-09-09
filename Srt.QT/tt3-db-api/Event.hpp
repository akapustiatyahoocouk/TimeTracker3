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
    /// \class OEvent "tt3-db-api/API.hpp"
    /// \brief A recorded event.
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
        virtual auto    type() const
                            -> IObjectType * override
        {
            return ObjectTypes::Event::instance();
        }

        //////////
        //  Operations (properties)
    public:
        /// \brief
        ///     Returns the UTC date+time when this event was logged.
        /// \return
        ///     The UTC date+time when this event was logged.
        /// \exception DatabaseException
        ///     If an error occurs.
        virtual auto    occurredAt(
                            ) const -> QDateTime = 0;

        /// \brief
        ///     Returns the summary of this event, as entered by the user.
        /// \return
        ///     The 1-line summary of this event, as entered by the user.
        /// \exception DatabaseException
        ///     If an error occurs.
        virtual auto    summary(
                            ) const -> QString = 0;

        //////////
        //  Operations (associations)
    public:
        /// \brief
        ///     Returns the User who has logged this event.
        /// \return
        ///     The User who has logged this event.
        /// \exception DatabaseException
        ///     If an error occurs.
        virtual auto    user(
                            ) const -> IUser * = 0;

        /// \brief
        ///     Returns the Activity against which this event was logged;.
        /// \return
        ///     The Activity against which this event was logged;
        ///     nullptr == none.
        /// \exception DatabaseException
        ///     If an error occurs.
        virtual auto    activity(
                            ) const -> IActivity * = 0;
    };
}

//  End of tt3-db-api/Work.hpp
