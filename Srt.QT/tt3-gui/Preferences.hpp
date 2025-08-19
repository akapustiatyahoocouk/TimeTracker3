//
//  tt3-gui/Preferences.hpp - Preferences framework
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

namespace tt3::gui
{
    class TT3_GUI_PUBLIC PreferencesEditor;

    //  The generic "preferences" - a node in the
    //  hierarchy of editable preferences.
    //  Normally the actual preferences in a given
    //  Preferences node will be acked uo via Settings
    //  of some components; Preferences just organize
    //  these settings into a hierarchy of editable
    //  setting groups.
    class TT3_GUI_PUBLIC Preferences
    {
        CANNOT_ASSIGN_OR_COPY_CONSTRUCT(Preferences)

        //////////
        //  Construction/destruction
    protected:
        Preferences() = default;
        virtual ~Preferences() = default;

        //////////
        //  Operations
    public:
        //  The mnemonic identifier of this preferences node
        virtual QString         mnemonic() const = 0;

        //  The user-readable display name of this preferences node
        virtual QString         displayName() const = 0;

        //  The immediate parent node of this Preferences
        //  ir null if this is one of the root nodes.
        virtual Preferences *   parent() const = 0;

        //  The set of immediate child nodes of this
        //  Preferences node.
        //  TODO create default implementation
        virtual QSet<Preferences*>  children() const;

        //  The relative order of this Preferences within its
        //  parent (lower orders come before higher orders; children
        //  with the same order are sorted by display name).
        virtual int             order() const { return INT_MAX; }

        //  Creates a new editor widget for this preferences node,
        //  or nullptr if this node is not editable
        virtual PreferencesEditor * createEditor() = 0;
    };

    //////////
    //  The registry of known Preferences
    class TT3_GUI_PUBLIC PreferencesRegistry final
    {
        UTILITY_CLASS(PreferencesRegistry)

        //////////
        //  Operationds
    public:
        //  TODO document
        static QSet<Preferences*>   allPreferences();
        static QSet<Preferences*>   rootPreferences();
        static bool                 registerPreferences(Preferences * preferences);
        static Preferences *        findPreferences(const QString & mnemonic);

        //////////
        //  Implementation
    private:
        static QMap<QString, Preferences*>  _registry;  //  key = mnemonic
    };
}

//  End of tt3-gui/Preferences.hpp

