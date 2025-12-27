//
//  tt3-skin-slim/Component.hpp - tt3-skin-slim Component
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
    /// \class Component tt3-skin-slim/API.hpp
    /// \brief The "TT3 slim skin" component.
    class TT3_SKIN_SLIM_PUBLIC Component final
        :   public virtual tt3::util::IComponent
    {
        TT3_DECLARE_COMPONENT(Component)

        //////////
        //  Types
    public:
        /// \class Resources tt3-skin-slim/API.hpp
        /// \brief The component's resources.
        class TT3_SKIN_SLIM_PUBLIC Resources final
            :   public tt3::util::FileResourceFactory
        {
            TT3_DECLARE_SINGLETON(Resources)
        };

        /// \class Settings tt3-skin-slim/API.hpp
        /// \brief The component's settings.
        class TT3_SKIN_SLIM_PUBLIC Settings final
            :   public tt3::util::Settings
        {
            TT3_DECLARE_SINGLETON(Settings)

            //////////
            //  Properties
        public:
            /// \brief
            ///     The "normal" bounds of the main UI frame
            ///     (that is, when it is noe minimized or maximized).
            tt3::util::Setting<QRect>   mainFrameBounds;

            /// \brief
            ///     True if the main UI frame should stay on top when visible.
            tt3::util::Setting<bool>    mainFrameAlwaysOnTop;

            /// \brief
            ///     The opacity of main UI frame, in % (0 == transparent, 100 == opaque).
            tt3::util::Setting<int>     mainFrameOpacity;
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
        virtual Resources *     resources() const override;
        virtual Settings *      settings() override;
        virtual const Settings *settings() const override;
        virtual void            initialize() override;
        virtual void            deinitialize() override;
    };
}

//  End of tt3-skin-slim/Component.hpp
