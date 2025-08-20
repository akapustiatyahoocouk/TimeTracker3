//
//  tt3-gui/GeneralStartupPreferencesEditor.hpp - The "/General/Startup" preferences editor
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
    //  The "/General/Startup" preferences editor
    namespace Ui { class GeneralStartupPreferencesEditor; }

    class TT3_GUI_PUBLIC GeneralStartupPreferencesEditor final : public PreferencesEditor
    {
        Q_OBJECT
        CANNOT_ASSIGN_OR_COPY_CONSTRUCT(GeneralStartupPreferencesEditor)

        //////////
        //  Construction/destruction
    public:
        explicit GeneralStartupPreferencesEditor(QWidget * parent);
        ~GeneralStartupPreferencesEditor();

        //////////
        //  PreferencesEditor
    public:
        virtual Preferences *   preferences() const override;
        virtual void    loadControlValues() override;
        virtual void    saveControlValues() override;
        virtual void    resetControlValues() override;
        virtual bool    isValid() const override;

        //////////
        //  Controls
    private:
        Ui::GeneralStartupPreferencesEditor *   _ui;
    };
}

#endif  //  TT3_GUI_PREFERENCES_EDITOR_DEFINED
//  End of tt3-gui/GeneralStartupPreferencesEditor.hpp
