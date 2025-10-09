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

namespace tt3::skin::admin
{
    /// \class Component tt3-skin-admin/API.hpp
    /// \brief The "TT3 Admin skin" component.
    class TT3_SKIN_ADMIN_PUBLIC Component final
        :   public virtual tt3::util::IComponent
    {
        DECLARE_SINGLETON(Component)

        //////////
        //  Types
    public:
        /// \class Resources tt3-skin-admin/API.hpp
        /// \brief The component's resources.
        class TT3_SKIN_ADMIN_PUBLIC Resources final
            :   public tt3::util::FileResourceFactory
        {
            DECLARE_SINGLETON(Resources)
        };

        /// \class Settings tt3-skin-admin/API.hpp
        /// \brief The component's settings.
        class TT3_SKIN_ADMIN_PUBLIC Settings final
            :   public tt3::util::Settings
        {
            DECLARE_SINGLETON(Settings)

            //////////
            //  Properties
        public:
            /// \brief
            ///     The "normal" bounds of the main UI frame
            ///     (that is, when it is noe minimized or maximized).
            tt3::util::Setting<QRect>   mainFrameBounds;

            /// \brief
            ///     True if the main UI frame is maximized, false if not.
            tt3::util::Setting<bool>    mainFrameMaximized;

            /// \brief
            ///     The current tab index within the MainFrame.
            tt3::util::Setting<int>     mainFrameCurrentTab;
        };

        //////////
        //  IComponent
    public:
        /// \brief A type alias to improve code readability.
        using Mnemonic = tt3::util::Mnemonic;
        /// \brief A type alias to improve code readability.
        using Subsystem = tt3::util::ISubsystem;

        virtual Mnemonic        mnemonic() const override;
        virtual QString         displayName() const override;
        virtual QString         description() const override;
        virtual QString         copyright() const override;
        virtual QVersionNumber  version() const override;
        virtual QString         buildNumber() const override;
        virtual Subsystem *     subsystem() const override;
        virtual Resources *     resources() const override;
        virtual Settings *      settings() override;
        virtual const Settings *settings() const override;
    };
}

//  End of tt3-skin-admin/Components.hpp
