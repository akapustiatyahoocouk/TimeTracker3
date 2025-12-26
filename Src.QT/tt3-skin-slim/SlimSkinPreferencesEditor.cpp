//
//  tt3-skin-slim/SlimSkinPreferencesEditor.cpp - tt3::skin::slim::SlimSkinPreferencesEditor class implementation
//  TODO Localize via Resources
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
#include "tt3-skin-slim/API.hpp"
#include "ui_SlimSkinPreferencesEditor.h"
using namespace tt3::skin::slim;

//////////
//  Constructuion/destruction
SlimSkinPreferencesEditor::SlimSkinPreferencesEditor(
        QWidget * parent
    ) : tt3::gui::PreferencesEditor(parent),
        _ui(new Ui::SlimSkinPreferencesEditor)
{
    _ui->setupUi(this);

    //  Start off with current values from Settings
    loadControlValues();
}

SlimSkinPreferencesEditor::~SlimSkinPreferencesEditor()
{
    delete _ui;
}

//////////
//  tt3::gui::PreferencesEditor
auto SlimSkinPreferencesEditor::preferences(
    ) const -> tt3::gui::Preferences *
{
    return SlimSkinPreferences::instance();
}

void SlimSkinPreferencesEditor::loadControlValues()
{
    _ui->alwaysOnTopCheckBox->setChecked(
        Component::Settings::instance()->mainFrameAlwaysOnTop);
}

void SlimSkinPreferencesEditor::saveControlValues()
{
    Component::Settings::instance()->mainFrameAlwaysOnTop =
        _ui->alwaysOnTopCheckBox->isChecked();
    //  Apply changes
    if (Skin::instance()->_mainFrame != nullptr)
    {
        Skin::instance()->_mainFrame->setAlwaysOnTop(
            _ui->alwaysOnTopCheckBox->isChecked());
    }
    //  TODO
}

void SlimSkinPreferencesEditor::resetControlValues()
{
    _ui->alwaysOnTopCheckBox->setChecked(
        Component::Settings::instance()->mainFrameAlwaysOnTop.defaultValue());
}

bool SlimSkinPreferencesEditor::isValid() const
{
    return true;
}

//  End of tt3-skin-slim/SlimSkinPreferencesEditor.cpp
