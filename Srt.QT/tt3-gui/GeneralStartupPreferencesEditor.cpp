//
//  tt3-gui/GeneralStartupPreferencesEditor.cpp - tt3::gui::GeneralAppearancePreferencesEditor class implementation
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
#include "ui_GeneralStartupPreferencesEditor.h"
using namespace tt3::gui;

//////////
//  Construction/destruction
GeneralStartupPreferencesEditor::GeneralStartupPreferencesEditor(QWidget * parent)
    :   PreferencesEditor(parent),
        _ui(new Ui::GeneralStartupPreferencesEditor)
{
    _ui->setupUi(this);
}

GeneralStartupPreferencesEditor::~GeneralStartupPreferencesEditor()
{
    delete _ui;
}

//////////
//  PreferencesEditor
Preferences * GeneralStartupPreferencesEditor::preferences() const
{
    return GeneralStartupPreferences::instance();
}

void GeneralStartupPreferencesEditor::loadControlValues()
{
    //  TODO implement
}

void GeneralStartupPreferencesEditor::saveControlValues()
{
    //  TODO implement
}

void GeneralStartupPreferencesEditor::resetControlValues()
{
    //  TODO implement
}

bool GeneralStartupPreferencesEditor::isValid() const
{
    return true;
}

//  End of tt3-gui/GeneralStartupPreferencesEditor.cpp
