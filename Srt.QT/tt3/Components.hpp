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
    //  The "TT3 application" component
    class Component final : public virtual tt3::util::IComponent
    {
        DECLARE_SINGLETON(Component)

        //////////
        //  Types
    public:
        //  The "TT3 application" component settings
        class Settings final : public tt3::util::Settings
        {
            DECLARE_SINGLETON(Settings)

            //////////
            //  Properties
        public:
        };

        //////////
        //  IComponent
    public:
        using Mnemonic = tt3::util::Mnemonic;
        using Subsystem = tt3::util::ISubsystem;
        using Resources = tt3::util::FileResourceFactory;

        virtual Mnemonic        mnemonic() const override;
        virtual QString         displayName() const override;
        virtual QString         description() const override;
        virtual QString         copyright() const override;
        virtual QVersionNumber  version() const override;
        virtual QString         buildNumber() const override;
        virtual Subsystem *     subsystem() const override;
        virtual Resources &     resources() override;
        virtual Settings &      settings() override;

        //////////
        //  Implementation
    private:
        Resources               _resources;
    };
}

//  End of tt3/Components.hpp
