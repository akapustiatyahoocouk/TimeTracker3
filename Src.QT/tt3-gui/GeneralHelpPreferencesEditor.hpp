//
//  tt3-gui/GeneralHelpPreferencesEditor.hpp - The "/General/Help" preferences editor
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

#ifdef TT3_GUI_PREFERENCES_EDITOR_DEFINED

namespace tt3::gui
{
    namespace Ui { class GeneralHelpPreferencesEditor; }

    /// \class GeneralHelpPreferencesEditor tt3-gui/API.hpp
    /// \brief The "General/Help" preferences editor.
    class TT3_GUI_PUBLIC GeneralHelpPreferencesEditor final
        :   public PreferencesEditor
    {
        Q_OBJECT
        TT3_CANNOT_ASSIGN_OR_COPY_CONSTRUCT(GeneralHelpPreferencesEditor)

        //////////
        //  Construction/destruction
    public:
        explicit GeneralHelpPreferencesEditor(QWidget *parent = nullptr);
        virtual ~GeneralHelpPreferencesEditor();

        //////////
        //  PreferencesEditor
    public:
        virtual auto    preferences() const -> Preferences * override;
        virtual void    loadControlValues() override;
        virtual void    saveControlValues() override;
        virtual void    resetControlValues() override;
        virtual bool    isValid() const override;

        //////////
        //  Controls
    private:
        Ui::GeneralHelpPreferencesEditor *const _ui;

        //////////
        //  Signal handlers
    private slots:
        void            _cacheHelpCollectionCheckBoxStateChanged(int);
        void            _systemBrowserRadioButtonClicked();
        void            _helpViewerRadioButtonClicked();
    };
}

#endif  //  def TT3_GUI_PREFERENCES_EDITOR_DEFINED
//  End of tt3-gui/GeneralHelpPreferencesEditor.hpp
