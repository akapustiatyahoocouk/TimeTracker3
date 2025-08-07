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

namespace tt3::util
{
    //  The "TT3 Util" component settings
    class TT3_UTIL_PUBLIC UtilSettings final : public Settings
    {
        DECLARE_SINGLETON(UtilSettings)

        //////////
        //  Properties
    public:
    };

    //  The "TT3 Util" component
    class TT3_UTIL_PUBLIC UtilComponent final : public virtual IComponent
    {
        DECLARE_SINGLETON(UtilComponent)

        //////////
        //  IComponent
    public:
        virtual QString         mnemonic() const override;
        virtual QString         displayName() const override;
        virtual QString         desciption() const override;
        virtual QString         copyright() const override;
        virtual QVersionNumber  version() const override;
        virtual QString         buildNumber() const override;
        virtual UtilSettings &  settings() override { return *UtilSettings::instance(); }

        //////////
        //  Implementation
    private:
    };
}

//  End of tt3-util/Components.hpp
