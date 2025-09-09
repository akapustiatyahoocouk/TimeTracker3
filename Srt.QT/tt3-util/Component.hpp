//
//  tt3-util/Component.cpp - Component framework
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
    /// \class IComponent API.hpp "tt3-util/API.hpp"
    /// \brief A component making up the TT3 architecture.
    /// \details
    ///     While a "plugin" is a unit of disctibution
    ///     1 DLL == 1 plugin), "components" are architectural
    ///     elements, so it may be not uncommon for a single
    ///     plugin to define (and register) several components.
    class TT3_UTIL_PUBLIC IComponent
    {
        //////////
        //  This is an interface
    protected:
        IComponent() = default;
        virtual ~IComponent() = default;

        //////////
        //  Operations
    public:
        /// \brief
        ///     Returns the mnemonic identifier of this component.
        /// \details
        ///     All components in a TT3 installation must have
        ///     different "mnemonics", which are not localizable.
        /// \return
        ///     The mnemonic identifier of this component.
        virtual auto    mnemonic(
                            ) const -> Mnemonic = 0;

        /// \brief
        ///     Returns the user-readable display name of this component.
        /// \return
        ///     The user-readable display name of this component
        ///     for the current default locale.
        virtual auto    displayName(
                            ) const -> QString = 0;

        /// \brief
        ///     Returns the short (1 line) user-readable description
        ///     of this component.
        /// \return
        ///     The short (1 line) user-readable description of
        ///     this component for the current default locale.
        virtual auto    description(
                            ) const -> QString = 0;

        /// \brief
        ///     Returns the copyight message of this component.
        /// \return
        ///     The copyight message of this component for
        ///     the current default locales.
        virtual auto    copyright(
                            ) const -> QString = 0;

        /// \brief
        ///     Returns the version of this component.
        /// \return
        ///     The version of this component.
        virtual auto    version(
                            ) const -> QVersionNumber = 0;

        /// \brief
        ///     Returns the build number of this component.
        /// \return
        ///     The build number of this component.
        virtual auto    buildNumber(
                            ) const -> QString = 0;

        /// \brief
        ///     Returns the small (16x16) icon representing this component.
        /// \return
        ///     The small (16x16) icon representing this component.
        virtual auto    smallIcon(
                            ) const -> QIcon;

        /// \brief
        ///     Returns the large (32x32) icon representing this component.
        /// \return
        ///     The large (32x32) icon representing this component.
        virtual auto    largeIcon(
                            ) const -> QIcon;

        /// \brief
        ///     Returns the license of this component.
        /// \details
        ///     The default implementation always returns GPLv3.
        /// \return
        ///     The license of this component.
        virtual auto    license(
                            ) const -> ILicense *;

        /// \brief
        ///     Returns the subsystem yo which this component belongs.
        /// \return
        ///     The subsystem yo which this component belongs.
        virtual auto    subsystem(
                            ) const -> ISubsystem * = 0;

        /// \brief
        ///     Returns the resource factory used by this component.
        /// \details
        ///     Normally each Component will have its own decicated
        ///     resource factory; however, several tightly coupled
        ///     components may choose to share the same resource factory.
        /// \return
        ///     The resource factory used by this component.
        virtual auto    resources(
                            ) const -> IResourceFactory * = 0;

        /// \brief
        ///     Returns the settings of this component.
        /// \return
        ///     The settings of this component.
        virtual auto    settings(
                            ) -> Settings * = 0;

        /// \brief
        ///     Returns the settings of this component.
        /// \return
        ///     The settings of this component.
        virtual auto    settings(
                            ) const -> const Settings * = 0;
    };

    /// \class ComponentManager API.hpp "tt3-util/API.hpp"
    /// \brief The manager of known components.
    class TT3_UTIL_PUBLIC ComponentManager final
    {
        UTILITY_CLASS(ComponentManager)

        //////////
        //  Operations
    public:
        /// \brief
        ///     Returns the set of all registered components.
        /// \return
        ///     Returns the set of all registered components.
        static auto     allComponents()
                            -> Components;

        /// \brief
        ///     Registers the specified component.
        /// \details
        ///     Registering an already-registered component
        ///     does nothing and returns true (success).
        /// \param component
        ///     The component to register.
        /// \return
        ///     True on success, false on failure.
        static bool     registerComponent(
                                IComponent * component
                            );

        /// \brief
        ///     Finds a registered component by mnemonic and version.
        /// \param mnemonic
        ///     The component mnemonic to look for.
        /// \param version
        ///     The component version to look for.
        /// \return
        ///     The registered component with the required mnemonic
        ///     and version or nullptr if not found.
        static auto     findComponent(
                                const Mnemonic & mnemonic,
                                const QVersionNumber & version
                            ) -> IComponent *;

        /// \brief
        ///     Finds a registered component by mnemonic.
        /// \details
        ///     If several versions of component with the same
        ///     mnemonic are registered, finds the one with the
        ///     latest version.
        /// \param mnemonic
        ///     The component mnemonic to look for.
        /// \return
        ///     The latest available version of a registered component
        ///     with the required mnemonic or nullptr if none found.
        static auto     findComponent(
                                const Mnemonic & mnemonic
                            ) -> IComponent *;

        /// \brief
        ///     Loads Settings of all registered components from
        ///     an application-wide text configuration file.
        /// \details
        ///     This file is a hidden file located in the user's
        ///     home directory.
        ///     Any settings not explicitly present in the configuration
        ///     file retain their default values.
        static void     loadComponentSettings();

        /// \brief
        ///     Saves Settings of all registered components to
        ///     an application-wide text configuration file.
        /// \details
        ///     This file is a hidden file located in the user's
        ///     home directory.
        static void     saveComponentSettings();

        /// \brief
        ///     Returns the set of all locales supported by at
        ///     least one registered component
        /// \return
        ///     The set of all locales supported by at
        ///     least one registered component
        static Locales  supportedLocales();

        /// \brief
        ///     Returns the set of all locales supported by
        ///     every registered component
        /// \return
        ///     The set of all locales supported by
        ///     every registered component.
        ///     NOTE, that this set may be empty.
        static Locales  fullySupportedLocales();

        //////////
        //  Implementation
    private:
        struct _Impl;

        //  Helpers
        static _Impl *  _impl();
    };
}

//  End of tt3-util/Component.cpp
