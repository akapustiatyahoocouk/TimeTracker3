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
        //  TODO document
        virtual QString         mnemonic() const = 0;
        virtual QString         displayName() const = 0;
        virtual QString         desciption() const = 0;
        virtual QString         copyright() const = 0;
        virtual QVersionNumber  version() const = 0;
        virtual QString         buildNumber() const = 0;

        //  The settings of this component
        virtual Settings &      settings() = 0;
    };

    //////////
    //  The registry of known components
    class TT3_UTIL_PUBLIC ComponentRegistry final
    {
        UTILITY_CLASS(ComponentRegistry)

        //////////
        //  Operationds
    public:
        //  TODO document
        static QSet<IComponent*>    allComponents();
        static bool                 registerComponent(IComponent * component);
        static IComponent *         findComponent(const QString & mnemonic, const QVersionNumber & version);
        static IComponent *         findComponent(const QString & mnemonic);    //  finds the latest version

        //////////
        //  Implementation
    private:
        static QMap<QString, IComponent*>   _registry;    //  key = "mnemonic" + "version"
    };
}

//  End of tt3-util/Component.cpp
