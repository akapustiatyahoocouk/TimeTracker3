//
//  tt3/Components.hpp - TT3 Components
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

namespace tt3
{
    //////////
    //  The "TT3 application" component settings
    class ApplicationSettings final : public tt3::util::Settings
    {
        DECLARE_SINGLETON(ApplicationSettings)

        //////////
        //  Properties
    public:
        //  TODO document
        tt3::util::Setting<bool>    reloadLastWorkspaceOnStartup;
    };

    //////////
    //  The "TT3 application" component
    class ApplicationComponent final : public tt3::util::Component
    {
        DECLARE_SINGLETON(ApplicationComponent)

        //////////
        //  Component
    public:
        virtual ApplicationSettings &   settings() override { return *ApplicationSettings::instance(); }

        //////////
        //  Implementation
    private:
    };
}

//  End of tt3/Components.hpp
