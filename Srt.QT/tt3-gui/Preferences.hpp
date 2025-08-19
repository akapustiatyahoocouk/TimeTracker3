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
#pragma once
#include "tt3-gui/API.hpp"

namespace tt3::gui
{
    //  The common base class for all "preferences editor" widgets
    class TT3_GUI_PUBLIC PreferencedEditor : public QWidget
    {
        Q_OBJECT
        CANNOT_ASSIGN_OR_COPY_CONSTRUCT(PreferencedEditor)

        //////////
        //  Construction/destruction
    public:
        explicit PreferencedEditor(QWidget * parent);
        virtual ~PreferencedEditor();

        //////////
        //  Operations
    public:
    };

    //  The generic "preferences" - a node in the
    //  hierarchy of editable preferences
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
        //  The user-readable display name of this preferences node
        virtual QString     displayName() const = 0;

        //  Creates a new editor widget for this preferences node,
        //  or nullptr if this node is not editable
        virtual PreferencedEditor * createEditor() = 0;
    };
}

//  End of tt3-gui/Preferences.hpp

