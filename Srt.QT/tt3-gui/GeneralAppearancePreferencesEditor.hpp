//
//  tt3-gui/GeneralAppearancePreferencesEditor.hpp - The "/General/Appearance" preferences editor
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
    class TT3_GUI_PUBLIC ISkin;
    class TT3_GUI_PUBLIC ITheme;

    //  The "/General/Appearance" preferences editor
    namespace Ui { class GeneralAppearancePreferencesEditor; }

    class TT3_GUI_PUBLIC GeneralAppearancePreferencesEditor final : public PreferencesEditor
    {
        Q_OBJECT
        CANNOT_ASSIGN_OR_COPY_CONSTRUCT(GeneralAppearancePreferencesEditor)

        //////////
        //  Construction/destruction
    public:
        explicit GeneralAppearancePreferencesEditor(QWidget * parent);
        virtual ~GeneralAppearancePreferencesEditor();

        //////////
        //  PreferencesEditor
    public:
        virtual Preferences *   preferences() const override;
        virtual void        loadControlValues() override;
        virtual void        saveControlValues() override;
        virtual void        resetControlValues() override;
        virtual bool        isValid() const override;

        //////////
        //  Implementation
    private:
        QList<QLocale>      _locales;   //  same order as in combo box
        QList<ISkin*>       _skins;     //  same order as in combo box
        QList<ITheme*>      _themes;    //  same order as in combo box

        //  Helpers
        QLocale             _selectedLocale();
        void                _setSelectedLocale(const QLocale & locale);
        tt3::gui::ISkin *   _selectedSkin();
        void                _setSelectedSkin(tt3::gui::ISkin * skin);
        tt3::gui::ITheme *  _selectedTheme();
        void                _setSelectedTheme(tt3::gui::ITheme * theme);

        //////////
        //  Controls
    private:
        Ui::GeneralAppearancePreferencesEditor *_ui;

        //////////
        //  Signal handlers
    private slots:
        void                _languageComboBoxCurrentIndexChanged(int);
        void                _skinComboBoxCurrentIndexChanged(int);
        void                _themeComboBoxCurrentIndexChanged(int);
    };
}

#endif  //  TT3_GUI_PREFERENCES_EDITOR_DEFINED
//  End of tt3-gui/GeneralAppearancePreferencesEditor.hpp
