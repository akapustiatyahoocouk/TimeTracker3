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
    /// \class IComponent tt3-util/API.hpp
    /// \brief A component making up the TT3 architecture.
    class TT3_UTIL_PUBLIC IComponent
    {
        friend class ComponentManager;

        //////////
        //  Types
    public:
        /// \brief A type alias to improve code readability.
        using Mnemonic = tt3::util::Mnemonic;
        /// \brief A type alias to improve code readability.
        using ISubsystem = tt3::util::ISubsystem;

        //////////
        //  This is an interface
    protected:
        IComponent() = default;
        virtual ~IComponent() noexcept = default;

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
        virtual QString displayName() const = 0;

        /// \brief
        ///     Returns the short (1 line) user-readable description
        ///     of this component.
        /// \return
        ///     The short (1 line) user-readable description of
        ///     this component for the current default locale.
        virtual QString description() const = 0;

        /// \brief
        ///     Returns the copyight message of this component.
        /// \return
        ///     The copyight message of this component for
        ///     the current default locales.
        virtual QString copyright() const = 0;

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
        virtual QString buildNumber() const = 0;

        /// \brief
        ///     Returns the small (16x16) icon representing this component.
        /// \return
        ///     The small (16x16) icon representing this component.
        virtual QIcon   smallIcon() const;

        /// \brief
        ///     Returns the large (32x32) icon representing this component.
        /// \return
        ///     The large (32x32) icon representing this component.
        virtual QIcon   largeIcon() const;

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

        /// \brief
        ///     Initializes this component; has no effect if
        ///     the component has already been initialized.
        /// \exception Exception
        ///     If the component initialization fails.
        virtual void    initialize() = 0;

        /// \brief
        ///     Deinitializes this component; has no effect if
        ///     the component has not been initialized.
        virtual void    deinitialize() = 0;

        //////////
        //  Implementation
    private:
        bool            _initialized = false;
    };

    /// \class ComponentManager tt3-util/API.hpp
    /// \brief The manager of known components.
    class TT3_UTIL_PUBLIC ComponentManager final
    {
        TT3_UTILITY_CLASS(ComponentManager)

        //////////
        //  Operations
    public:
        /// \brief
        ///     Returns the set of all registered components.
        /// \return
        ///     Returns the set of all registered components.
        static auto     allComponents(
                            ) -> Components;

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
        ///     Un-registers the specified component.
        /// \param component
        ///     The component to un-register.
        /// \return
        ///     True on success, false on failure.
        static bool     unregisterComponent(
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
        ///     Loads (bit does NOT initialize) optional Components
        ///     available in a TT3 installation.
        /// \details
        ///     This is done by loading those .DLL/.SO files in the
        ///     directory where the TT3 executable was launched from
        ///     which define Components.
        static void     loadOptionalComponents();

        /// \brief
        ///     Initializes all known Components by calling
        ///     initialize() on each of them.
        /// \details
        ///     Any exceptions thrown by Component initializers
        ///     are logged, but do not stop the initialization
        ///     process. This was as many Cmponents as possible
        ///     are initialized.
        static void     initializeComponents();

        /// \brief
        ///     Deinitializes all known Components by calling
        ///     deinitialize() on each of them.
        static void     deinitializeComponents();

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
        static void     _loadLibrary(const QString & fileName);
    };

//  A helper macro for Component declaration - use within a .hpp
//  file in a declaration of an IComponent - implementing class
#define TT3_DECLARE_COMPONENT(Clazz)    \
    TT3_DECLARE_SINGLETON(Clazz)        \
public:                                 \
    class Registrator final             \
    {                                   \
        TT3_CANNOT_ASSIGN_OR_COPY_CONSTRUCT(Registrator)    \
    public:                             \
        Registrator();                  \
        ~Registrator();                 \
    };

//  A helper macro for Component definition - use
//  within a .cpp file where the IComponent -
//  implementing class is defined
#define TT3_IMPLEMENT_COMPONENT(Clazz)  \
    TT3_IMPLEMENT_SINGLETON(Clazz)      \
    Clazz::Clazz() {}                   \
    Clazz::~Clazz() {}                  \
    Clazz::Registrator::Registrator()   \
    {                                   \
        tt3::util::ComponentManager::registerComponent(Clazz::instance());  \
    }                                   \
    Clazz::Registrator::~Registrator()  \
    {                                   \
        tt3::util::ComponentManager::unregisterComponent(Clazz::instance());\
    }                                   \
    namespace                           \
    {                                   \
        Clazz::Registrator the##Clazz##Registrator; \
    }

    //  The #ifdef guard is needed for MOC to work properly
#ifdef TT3_UTIL_SETTINGS_DEFINED
    /// \class Component tt3-util/API.hpp
    /// \brief The "TT3 Util" component
    class TT3_UTIL_PUBLIC Component final
        :   public virtual IComponent
    {
        TT3_DECLARE_COMPONENT(Component)

        //////////
        //  Types
    public:
        //  The component's resources
        class TT3_UTIL_PUBLIC Resources final
            :   public FileResourceFactory
        {
            TT3_DECLARE_SINGLETON(Resources)
        };

        //  The component's settings
        class TT3_UTIL_PUBLIC Settings final
            :   public tt3::util::Settings
        {
            TT3_DECLARE_SINGLETON(Settings)

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
        virtual Resources *     resources() const override;
        virtual Settings *      settings() override;
        virtual const Settings *settings() const override;
        virtual void            initialize() override;
        virtual void            deinitialize() override;
    };
#endif //   def TT3_UTIL_SETTINGS_DEFINED
}

//  End of tt3-util/Component.cpp
