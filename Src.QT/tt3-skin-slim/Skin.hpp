//
//  tt3-skin-slim/Skin.hpp - tt3 slim skin
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

namespace tt3::skin::slim
{
    class SlimSkinPreferencesEditor;
    class MainFrame;

    /// \class Skin tt3-skin-slim/API.hpp
    /// \brief The "TT3 slim skin".
    class TT3_SKIN_SLIM_PUBLIC Skin final
        :   public virtual tt3::gui::ISkin
    {
        TT3_DECLARE_SINGLETON(Skin)

        friend class SlimSkinPreferencesEditor;

        //////////
        //  gui::ISkin (properties)
    public:
        virtual auto    mnemonic() const -> Mnemonic override;
        virtual QString displayName() const override;
        virtual QString description() const override;
        virtual QIcon   smallIcon() const override;
        virtual QIcon   largeIcon() const override;
        virtual bool    isDefault() const override;

        //////////
        //  gui::ISkin (state)
    public:
        virtual bool    isActive() const override;
        virtual void    activate() override;
        virtual void    deactivate() override;
        virtual QWidget*dialogParent() override;

        //////////
        //  Implementation
    private:
        //  nullptr when skin is inactive,
        //  invisible when minimized to system tray
        MainFrame *     _mainFrame = nullptr;
    };
}

//  End of tt3-skin-slim/Skin.hpp
