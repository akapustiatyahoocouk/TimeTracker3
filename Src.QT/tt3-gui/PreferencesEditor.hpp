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

    /// \class PreferencesEditor tt3-gui/API.hpp
    /// \brief The common base class for all "preferences editor" widgets.
    class TT3_GUI_PUBLIC PreferencesEditor
        :   public QWidget
    {
        Q_OBJECT
        TT3_CANNOT_ASSIGN_OR_COPY_CONSTRUCT(PreferencesEditor)

        //////////
        //  Construction/destruction
    public:
        /// \brief
        ///     Constructs the widget.
        /// \param parent
        ///     The parent for the new widget; nullptr == none.
        explicit PreferencesEditor(
                QWidget * parent
            );

        /// \brief
        ///     The class destructor.
        virtual ~PreferencesEditor();

        //////////
        //  Operations
    public:
        /// \brief
        ///     Returns the Preferences edited by this editor.
        /// \return
        ///     The Preferences edited by this editor.
        virtual Preferences *   preferences() const = 0;

        /// \brief
        ///     Populates inner widgets with values taken
        ///     from persistent settings.
        virtual void    loadControlValues() = 0;

        /// \brief
        ///     Saves current values of inner widgets to
        ///     persistent settings IF THESE ARE ALL VALID,
        ///     ELSE DOES NOT SAVE ANY OF THEM.
        virtual void    saveControlValues() = 0;

        /// \brief
        ///     Resets the values of inner widgets to their defaults.
        virtual void    resetControlValues() = 0;

        /// \brief
        ///     Checks whether all inner widgets have values
        ///     that can be saved to persistent settings.
        /// \details
        ///     All text fields that allow only numeric values
        ///     have only valid numbers in them, etc.
        /// \return
        ///     True if all inner widgets have values that can
        ///     be saved to persistent settings, else false.
        virtual bool    isValid() const = 0;

        //////////
        //  Signals
    signals:
        /// \brief
        ///     Emitted when the value of some inner widget changes.
        void            controlValueChanged();
    };
}

//  Macro needed for MOC-generated .cpp files
#define TT3_GUI_PREFERENCES_EDITOR_DEFINED

//  End of tt3-gui/PreferencesEditor.hpp

