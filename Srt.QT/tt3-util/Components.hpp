//
//  tt3-util/Components.hpp - TT3-util Components
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

#ifdef TT3_UTIL_SETTINGS_DEFINED

namespace tt3::util
{
    //  The "TT3 Util" component
    class TT3_UTIL_PUBLIC Component final : public virtual IComponent
    {
        DECLARE_SINGLETON(Component)

        //////////
        //  Types
    public:
        //  The "TT3 Util" component settings
        class TT3_UTIL_PUBLIC Settings final : public tt3::util::Settings
        {
            DECLARE_SINGLETON(Settings)

            //////////
            //  Properties
        public:
        };

        //////////
        //  IComponent
    public:
        virtual Mnemonic        mnemonic() const override;
        virtual QString         displayName() const override;
        virtual QString         description() const override;
        virtual QString         copyright() const override;
        virtual QVersionNumber  version() const override;
        virtual QString         buildNumber() const override;
        virtual ISubsystem *    subsystem() const override;
        virtual Settings &      settings() override { return *Settings::instance(); }

        //////////
        //  Implementation
    private:
    };
}

#endif  //  def TT3_UTIL_SETTINGS_DEFINED
//  End of tt3-util/Components.hpp
