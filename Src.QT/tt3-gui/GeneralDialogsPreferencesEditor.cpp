//
//  tt3-gui/GeneralDialogsPreferencesEditor.cpp - tt3::gui::GeneralDialogsPreferencesEditor class implementation
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
#include "ui_GeneralDialogsPreferencesEditor.h"
using namespace tt3::gui;

//////////
//  Construction/destruction
GeneralDialogsPreferencesEditor::GeneralDialogsPreferencesEditor(QWidget * parent)
    :   PreferencesEditor(parent),
        _ui(new Ui::GeneralDialogsPreferencesEditor)
{
    tt3::util::ResourceReader rr(Component::Resources::instance(), RSID(GeneralDialogsPreferencesEditor));

    _ui->setupUi(this);

    _ui->confirmationsGroupBox->setTitle(
        rr.string(RID(ConfirmationsGroupBox)));
    _ui->confirmCloseWorkspaceCheckBox->setText(
        rr.string(RID(ConfirmCloseWorkspaceCheckBox)));
    _ui->confirmRestartCheckBox->setText(
        rr.string(RID(ConfirmRestartCheckBox)));
    _ui->confirmExitCheckBox->setText(
        rr.string(RID(ConfirmExitCheckBox)));

    //  Start off with current values from Settings
    loadControlValues();
}

GeneralDialogsPreferencesEditor::~GeneralDialogsPreferencesEditor()
{
    delete _ui;
}

//////////
//  PreferencesEditor
Preferences * GeneralDialogsPreferencesEditor::preferences() const
{
    return GeneralAppearancePreferences::instance();
}

void GeneralDialogsPreferencesEditor::loadControlValues()
{
    _ui->confirmCloseWorkspaceCheckBox->setChecked(
        Component::Settings::instance()->confirmCloseWorkspace);
    _ui->confirmRestartCheckBox->setChecked(
        Component::Settings::instance()->confirmRestart);
    _ui->confirmExitCheckBox->setChecked(
        Component::Settings::instance()->confirmExit);
}

void GeneralDialogsPreferencesEditor::saveControlValues()
{
    Component::Settings::instance()->confirmCloseWorkspace =
        _ui->confirmCloseWorkspaceCheckBox->isChecked();
    Component::Settings::instance()->confirmRestart =
        _ui->confirmRestartCheckBox->isChecked();
    Component::Settings::instance()->confirmExit =
        _ui->confirmExitCheckBox->isChecked();
}

void GeneralDialogsPreferencesEditor::resetControlValues()
{
    _ui->confirmCloseWorkspaceCheckBox->setChecked(
        Component::Settings::instance()->confirmCloseWorkspace.defaultValue());
    _ui->confirmRestartCheckBox->setChecked(
        Component::Settings::instance()->confirmRestart.defaultValue());
    _ui->confirmExitCheckBox->setChecked(
        Component::Settings::instance()->confirmExit.defaultValue());
}

bool GeneralDialogsPreferencesEditor::isValid() const
{
    return true;
}

//  End of tt3-gui/GeneralDialogsPreferencesEditor.cpp
