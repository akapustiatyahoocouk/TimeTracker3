//
//  tt3-gui/PreferencesEditor.hpp - Preferences editor
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
    class TT3_GUI_PUBLIC Preferences;

    //  The common base class for all "preferences editor" widgets
    class TT3_GUI_PUBLIC PreferencesEditor : public QWidget
    {
        Q_OBJECT
        CANNOT_ASSIGN_OR_COPY_CONSTRUCT(PreferencesEditor)

        //////////
        //  Construction/destruction
    public:
        explicit PreferencesEditor(QWidget * parent);
        virtual ~PreferencesEditor();

        //////////
        //  Operations
    public:
        //  The Preferences edited by this editor
        virtual Preferences *   preferences() const = 0;

        //  Populates inner widgets with values taken
        //  from persistent settings
        virtual void    loadControlValues() = 0;

        //  Saves current values of inner widgets to
        //  persistent settings IF THESE ARE ALL VALID,
        //  ELSE DOES NOT SAVE ANY OF THEM
        virtual void    saveControlValues() = 0;

        //  Resets the values of inner widgets to their defaults
        virtual void    resetControlValues() = 0;

        //  Checks whether all inner widgets have values
        //  that can be saved to persistent settings (e.h.
        //  text fields that allow only numeric values
        //  have only valid numbers in them, etc.)
        virtual bool    isValid() const = 0;
    };
}

//  Macro needed for MOC-generated .cpp files
#define TT3_GUI_PREFERENCES_EDITOR_DEFINED

//  End of tt3-gui/PreferencesEditor.hpp

