//
//  tt3-skin-admin/Components.hpp - tt3-skin-admin Components
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

namespace skin::admin
{
    //  The "TT3 Admin skin" component settings
    class TT3_SKIN_ADMIN_PUBLIC AdminSkinSettings final : public tt3::util::Settings
    {
        DECLARE_SINGLETON(AdminSkinSettings)

        //////////
        //  Properties
    public:
        //  TODO document
        tt3::util::Setting<QRect>   mainFrameBounds;
        tt3::util::Setting<bool>    mainFrameMaximized;
    };

    //  The "TT3 Admin skin" component
    class TT3_SKIN_ADMIN_PUBLIC AdminSkinComponent final : public virtual tt3::util::IComponent
    {
        DECLARE_SINGLETON(AdminSkinComponent)

        //////////
        //  IComponent
    public:
        virtual QString         mnemonic() const override;
        virtual QString         displayName() const override;
        virtual QString         desciption() const override;
        virtual QString         copyright() const override;
        virtual QVersionNumber  version() const override;
        virtual QString         buildNumber() const override;
        virtual AdminSkinSettings & settings() override { return *AdminSkinSettings::instance(); }

        //////////
        //  Implementation
    private:
    };
}

//  End of tt3-skin-admin/Components.hpp
