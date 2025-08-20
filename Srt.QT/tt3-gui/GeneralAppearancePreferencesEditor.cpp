//
//  tt3-gui/GeneralStartupPreferencesEditor.cpp - tt3::gui::GeneralStartupPreferencesEditor class implementation
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
#include "tt3-gui/API.hpp"
#include "ui_GeneralAppearancePreferencesEditor.h"
using namespace tt3::gui;

//////////
//  Construction/destruction
GeneralAppearancePreferencesEditor::GeneralAppearancePreferencesEditor(QWidget *parent)
    :   PreferencesEditor(parent),
        _ui(new Ui::GeneralAppearancePreferencesEditor)
{
    _ui->setupUi(this);

    //  Fill the language combo box with available locales
    //  TODO properly - from the set of locales supported by components
    _locales.append(Component::Settings::instance()->uiLocale.defaultValue());

    for (QLocale locale : _locales)
    {
        _ui->languageComboBox->addItem(
            QLocale::languageToString(locale.language()) +
            " (" +
            QLocale::territoryToString(locale.territory()) +
            ")");
    }
}

GeneralAppearancePreferencesEditor::~GeneralAppearancePreferencesEditor()
{
    delete _ui;
}

//////////
//  PreferencesEditor
Preferences * GeneralAppearancePreferencesEditor::preferences() const
{
    return GeneralAppearancePreferences::instance();
}

void GeneralAppearancePreferencesEditor::loadControlValues()
{
    //  TODO implement
}

void GeneralAppearancePreferencesEditor::saveControlValues()
{
    //  TODO implement
}

void GeneralAppearancePreferencesEditor::resetControlValues()
{
    //  TODO implement
}

bool GeneralAppearancePreferencesEditor::isValid() const
{
    return true;
}

//  End of tt3-gui/GeneralAppearancePreferencesEditor.cpp
