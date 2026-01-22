//
//  tt3-gui/GeneralHelpPreferencesEditor.cpp - tt3::gui::GeneralHelpPreferencesEditor class implementation
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
#include "ui_GeneralHelpPreferencesEditor.h"
using namespace tt3::gui;

//////////
//  Construction/destruction
GeneralHelpPreferencesEditor::GeneralHelpPreferencesEditor(
        QWidget * parent
    ) : PreferencesEditor(parent),
        _ui(new Ui::GeneralHelpPreferencesEditor)
{
    tt3::util::ResourceReader rr(Component::Resources::instance(), RSID(GeneralHelpPreferencesEditor));

    _ui->setupUi(this);

    //  Set static control values
    _ui->cacheHelpCollectionCheckBox->setText(
        rr.string(RID(CacheHelpCollectionCheckBox)));
    _ui->viewHelpInLabel->setText(
        rr.string(RID(ViewHelpInLabel)));
    _ui->systemBrowserRadioButton->setText(
        rr.string(RID(SystemBrowserRadioButton)));
    _ui->helpVieverRadioButton->setText(
        rr.string(RID(HelpVieverRadioButton)));

    //  Start off with current values from Settings
    loadControlValues();
}

GeneralHelpPreferencesEditor::~GeneralHelpPreferencesEditor()
{
    delete _ui;
}

//////////
//  PreferencesEditor
auto GeneralHelpPreferencesEditor::preferences() const -> Preferences *
{
    return GeneralHelpPreferences::instance();
}

void GeneralHelpPreferencesEditor::loadControlValues()
{
    _ui->cacheHelpCollectionCheckBox->setChecked(
        Component::Settings::instance()->cacheHelpCollection);
    _ui->systemBrowserRadioButton->setChecked(
        Component::Settings::instance()->helpAgent == HelpAgent::SystemBrowser);
    _ui->helpVieverRadioButton->setChecked(
        Component::Settings::instance()->helpAgent == HelpAgent::HelpViewer);
}

void GeneralHelpPreferencesEditor::saveControlValues()
{
    Component::Settings::instance()->cacheHelpCollection =
        _ui->cacheHelpCollectionCheckBox->isChecked();
    Component::Settings::instance()->helpAgent =
        _ui->helpVieverRadioButton->isChecked() ?
            HelpAgent::HelpViewer :
            HelpAgent::SystemBrowser;
}

void GeneralHelpPreferencesEditor::resetControlValues()
{
    _ui->cacheHelpCollectionCheckBox->setChecked(
        Component::Settings::instance()->cacheHelpCollection.defaultValue());
    _ui->systemBrowserRadioButton->setChecked(
        Component::Settings::instance()->helpAgent.defaultValue() == HelpAgent::SystemBrowser);
    _ui->helpVieverRadioButton->setChecked(
        Component::Settings::instance()->helpAgent.defaultValue() == HelpAgent::HelpViewer);
}

bool GeneralHelpPreferencesEditor::isValid() const
{
    return true;
}

//////////
//  Signal handlers
void GeneralHelpPreferencesEditor::_cacheHelpCollectionCheckBoxStateChanged(int)
{
    emit controlValueChanged();
}

void GeneralHelpPreferencesEditor::_systemBrowserRadioButtonClicked()
{
    emit controlValueChanged();
}

void GeneralHelpPreferencesEditor::_helpViewerRadioButtonClicked()
{
    emit controlValueChanged();
}

//  End of tt3-gui/GeneralHelpPreferencesEditor.cpp
