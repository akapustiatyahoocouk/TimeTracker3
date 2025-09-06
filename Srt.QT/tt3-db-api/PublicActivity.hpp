//
//  tt3-db-api/PublicActivity.hpp - a public activity
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
    //  A public  activity
    class TT3_DB_API_PUBLIC IPublicActivity : public virtual IActivity
    {
        //////////
        //  This is an interface
    protected:
        IPublicActivity() = default;
        virtual ~IPublicActivity() = default;

        //////////
        //  IObject
    public:
        virtual IObjectType *   type() const override
        {
            return ObjectTypes::PublicActivity::instance();
        }
    };
}

//  End of tt3-db-api/Activity.hpp
