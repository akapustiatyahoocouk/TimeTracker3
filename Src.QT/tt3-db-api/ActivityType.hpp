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
    /// \class IActivityType tt3-db-api/API.hpp
    /// \brief An activity type.
    class TT3_DB_API_PUBLIC IActivityType
        :   public virtual IObject
    {
        //////////
        //  This is an interface
    protected:
        IActivityType() = default;
        virtual ~IActivityType() = default;

        //////////
        //  IObject
    public:
        virtual auto    type(
                            ) const -> IObjectType * override
        {
            return ObjectTypes::ActivityType::instance();
        }

        //////////
        //  Operations (properties)
    public:
        /// \brief
        ///     Returns the user-readable display name of
        ///     this activity type.
        /// \return
        ///     The user-readable display name of this activity type.
        /// \exception DatabaseException
        ///     If an error occurs.
        virtual QString displayName() const = 0;

        /// \brief
        ///     Sets the user-readable display name of
        ///     this activity type.
        /// \param displayName
        ///     The new user-readable display name for this activity type.
        /// \exception DatabaseException
        ///     If an error occurs.
        virtual void    setDisplayName(
                                const QString & displayName
                            ) = 0;

        /// \brief
        ///     Returns the multi-line description of this activity type.
        /// \return
        ///     The multi-line description of this activity type,
        ///     with lines separated by a newline character '\\n'.
        /// \exception DatabaseException
        ///     If an error occurs.
        virtual QString description() const = 0;

        /// \brief
        ///     Sets the multi-line description of this activity type.
        /// \param description
        ///     The new multi-line description for this activity type,
        ///     with lines separated by a newline character '\\n'.
        /// \exception DatabaseException
        ///     If an error occurs.
        virtual void    setDescription(
                                const QString & description
                            ) = 0;

        //////////
        //  Operations (associations)
    public:
        /// \brief
        ///     Returns the set of all activities assigned to this activity type.
        /// \return
        ///     The set of all activities assigned to this activity type.
        /// \exception DatabaseException
        ///     If an error occurs.
        virtual auto    activities(
                            ) const -> Activities = 0;
    };
}

//  End of tt3-db-api/ActivityType.hpp
