//
//  tt3-skin-admin/AdminSkin.hpp - tt3 Administrator skin
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
    //////////
    //  The "TT3 Administrator skin"
    class TT3_SKIN_ADMIN_PUBLIC AdminSkin final : public virtual gui::ISkin
    {
        DECLARE_SINGLETON(AdminSkin)

        //////////
        //  gui::ISkin
    public:
        virtual QString     mnemonic() const override;
        virtual QString     displayName() const override;
        virtual QString     desciption() const override;
        virtual QIcon       smallIcon() const override;
        virtual QIcon       largeIcon() const override;
        virtual bool        isDefault() const override;

        //////////
        //  Implementation
    private:
        const QIcon         _smallIcon;
        const QIcon         _largeIcon;
    };
}

//  End of tt3-skin-admin/AdminSkin.hpp
