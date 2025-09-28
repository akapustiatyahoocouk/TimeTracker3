//
//  tt3-db-api/PrivateActivity.hpp - a private activity
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
    /// \class IPrivateActivity tt3-db-api/API.hpp
    /// \brief A private  activity
    class TT3_DB_API_PUBLIC IPrivateActivity
        :   public virtual IActivity
    {
        //////////
        //  This is an interface
    protected:
        IPrivateActivity() = default;
        virtual ~IPrivateActivity() = default;

        //////////
        //  IObject
    public:
        virtual auto    type(
                            ) const -> IObjectType * override
        {
            return ObjectTypes::PrivateActivity::instance();
        }

        //////////
        //  Operations (associations)
    public:
        /// \brief
        ///     Returns the owner of this private activity.
        /// \return
        ///     The owner of this private activity; never nullptr.
        /// \exception DatabaseException
        ///     If an error occurs.
        virtual auto    owner(
                            ) const -> IUser * = 0;
    };
}

//  End of tt3-db-api/Activity.hpp
