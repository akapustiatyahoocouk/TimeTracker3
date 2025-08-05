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

namespace util
{
    //////////
    //  A "component".
    //  While a "plugin" is a unit of disctibution (e.g. 1 DLL == 1 plugin),
    //  "components" are architectural elements, so it may be not uncommon
    //  for a single plugin to define (and register) several components.
    class TT3_UTIL_PUBLIC Component
    {
        CANNOT_ASSIGN_OR_COPY_CONSTRUCT(Component)

        //////////
        //  Construction/destruction
    public:
        Component(const QString & mnemonic, const QString & displayName,
                  const QString & description, const QString & copyright,
                  const QVersionNumber & version)
            :   _mnemonic(mnemonic),
                _displayName(displayName),
                _desciption(description),
                _copyright(copyright),
                _version(version) {}
        virtual ~Component() = default;

        //////////
        //  Operations
    public:
        //  TODO document
        virtual QString         mnemonic() const { return _mnemonic; }
        virtual QString         displayName() const { return _displayName; }
        virtual QString         desciption() const { return _desciption; }
        virtual QString         copyright() const { return _copyright; }
        virtual QVersionNumber  version() const { return _version; }
        virtual QString         buildNumber() const { return __DATE__; }

        //  The settings of this component
        virtual Settings &      settings() = 0;

        //////////
        //  Implementation
    private:
        QString                 _mnemonic;
        QString                 _displayName;
        QString                 _desciption;
        QString                 _copyright;
        QVersionNumber          _version;
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
        static QSet<Component*>     allComponents();
        static bool                 registerComponent(Component * component);
        static Component *          findComponent(const QString & mnemonic, const QVersionNumber & version);
        static Component *          findComponent(const QString & mnemonic);    //  finds the latest version

        //////////
        //  Implementation
    private:
        static QMap<QString, Component*>    _registry;  //  key = "mnemonic" + "version"
    };
}

//  End of tt3-util/Component.cpp
