//
//  tt3-db-api/Components.hpp - tt3-db-api Components
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

namespace db::api
{
    //  The "TT3 DB API" component settings
    class TT3_DB_API_PUBLIC DbApiSettings final : public util::Settings
    {
        DECLARE_SINGLETON(DbApiSettings)

        //////////
        //  Properties
    public:
    };

    //  The "TT3 DB API" component
    class TT3_DB_API_PUBLIC DbApiComponent final : public util::Component
    {
        DECLARE_SINGLETON(DbApiComponent)

        //////////
        //  Component
    public:
        virtual DbApiSettings & settings() override { return *DbApiSettings::instance(); }

        //////////
        //  Implementation
    private:
    };
}

EXECUTE_ONCE(util::ComponentRegistry::registerComponent(db::api::DbApiComponent::instance()))

//  End of tt3-db-api/Components.hpp
