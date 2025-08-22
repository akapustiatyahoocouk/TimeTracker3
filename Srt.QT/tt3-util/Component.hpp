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
    class TT3_UTIL_PUBLIC Settings;

    //////////
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
        virtual QString         mnemonic() const = 0;

        //  The user-readable display name of this component.
        virtual QString         displayName() const = 0;

        //  The user-readable description of this component.
        virtual QString         desciption() const = 0;

        //  The copyight message of this component.
        virtual QString         copyright() const = 0;

        //  The version of this component.
        virtual QVersionNumber  version() const = 0;

        //  The build number of this component.
        virtual QString         buildNumber() const = 0;

        //  The settings of this component
        virtual Settings &      settings() = 0;
    };

    //////////
    //  The manager of known components
    class TT3_UTIL_PUBLIC ComponentManager final
    {
        UTILITY_CLASS(ComponentManager)

        //////////
        //  Operationds
    public:
        //  Returns the set of all registered components.
        static QSet<IComponent*>    allComponents();

        //  Registers the specified component; returns true
        //  on success, false on failure.
        static bool                 registerComponent(IComponent * component);

        //  Finds a registered component by mnemonic and version.
        static IComponent *         findComponent(const QString & mnemonic, const QVersionNumber & version);

        //  Finds a registered component by mnemonic; if several versions
        //  of component with the same mnemonic are registered, returns the
        //  one with the latest version.
        static IComponent *         findComponent(const QString & mnemonic);    //  finds the latest version

        //  Loads Settings of all registered components from a text configuration file.
        static void                 loadComponentSettings();

        //  Saves Settings of all registered components to a text configuration file.
        static void                 saveComponentSettings();

        //////////
        //  Implementation
    private:
        static QMap<QString, IComponent*>   _registry;    //  key = "mnemonic" + "version"
    };
}

//  End of tt3-util/Component.cpp
