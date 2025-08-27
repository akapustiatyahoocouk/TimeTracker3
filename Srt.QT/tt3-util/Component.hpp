//
//  tt3-util/Component.cpp - Component framework
//
//  YimeTracker3
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
    //  A "component".
    //  While a "plugin" is a unit of disctibution (e.g. 1 DLL == 1 plugin),
    //  "components" are architectural elements, so it may be not uncommon
    //  for a single plugin to define (and register) several components.
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
        //  The mnemonic identifier of this component.
        virtual Mnemonic        mnemonic() const = 0;

        //  The user-readable display name of this component
        //  for the current default locale.
        virtual QString         displayName() const = 0;

        //  The short (1 line) user-readable description of
        //  this component for the current default locale.
        virtual QString         description() const = 0;

        //  The copyight message of this component for
        //  the current default locales.
        virtual QString         copyright() const = 0;

        //  The version of this component.
        virtual QVersionNumber  version() const = 0;

        //  The build number of this component.
        virtual QString         buildNumber() const = 0;

        //  The small (16x16) icon representing this component.
        virtual QIcon           smallIcon() const;

        //  The large (32x32) icon representing this component.
        virtual QIcon           largeIcon() const;

        //  The license of this component (default GPLv3).
        virtual ILicense *      license() const;

        //  The subsystem yo which this component belongs.
        virtual ISubsystem *    subsystem() const = 0;

        //  The resource factory used by this component.
        //  TODO "const" variant too ?
        virtual IResourceFactory &  resources() = 0;

        //  The settings of this component
        //  TODO "const" variant too ?
        virtual Settings &      settings() = 0;
    };

    //  The manager of known components
    class TT3_UTIL_PUBLIC ComponentManager final
    {
        UTILITY_CLASS(ComponentManager)

        //////////
        //  Operations
    public:
        //  Returns the set of all registered components.
        static Components   allComponents();

        //  Registers the specified component; returns true
        //  on success, false on failure.
        static bool         registerComponent(IComponent * component);

        //  Finds a registered component by mnemonic and version.
        static IComponent * findComponent(const QString & mnemonic, const QVersionNumber & version);

        //  Finds a registered component by mnemonic; if several versions
        //  of component with the same mnemonic are registered, returns the
        //  one with the latest version.
        static IComponent * findComponent(const QString & mnemonic);    //  finds the latest version

        //  Loads Settings of all registered components from a text configuration file.
        static void         loadComponentSettings();

        //  Saves Settings of all registered components to a text configuration file.
        static void         saveComponentSettings();

        //////////
        //  Implementation
    private:
        static tt3::util::Mutex _guard;
        static QMap<QString, IComponent*>   _registry;    //  key = "mnemonic" + "version"
    };
}

//  End of tt3-util/Component.cpp
