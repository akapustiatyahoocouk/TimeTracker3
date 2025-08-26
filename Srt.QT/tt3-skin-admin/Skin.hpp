//
//  tt3-skin-admin/Skin.hpp - tt3 Administrator skin
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

namespace tt3::skin::admin
{
    //  TODO move these to Classes.hpp
    class TT3_SKIN_ADMIN_PUBLIC MainFrame;

    //////////
    //  The "TT3 Administrator skin"
    class TT3_SKIN_ADMIN_PUBLIC Skin final : public virtual tt3::gui::ISkin
    {
        DECLARE_SINGLETON(Skin)

        //////////
        //  gui::ISkin (properties)
    public:
        virtual QString     mnemonic() const override;
        virtual QString     displayName() const override;
        virtual QString     description() const override;
        virtual QIcon       smallIcon() const override;
        virtual QIcon       largeIcon() const override;
        virtual bool        isDefault() const override;

        //////////
        //  gui::ISkin (state)
    public:
        virtual bool        isActive() const override;
        virtual void        activate() override;
        virtual void        deactivate() override;
        virtual QMainWindow*mainWindow() override;

        //////////
        //  Implementation
    private:
        const QIcon         _smallIcon;
        const QIcon         _largeIcon;

        MainFrame *         _mainFrame = nullptr;   //  nullptr when skin is inactive
    };
}

//  End of tt3-skin-admin/Skin.hpp
