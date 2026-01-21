//
//  tt3-db-xml/PreferencesEditor.cpp - tt3::db::xml::PreferencesEditor class implementation
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
#include "tt3-db-xml/API.hpp"
#include "ui_PreferencesEditor.h"
using namespace tt3::db::xml;

//////////
//  Construction/destruction
PreferencesEditor::PreferencesEditor(
        QWidget *parent
    ) : tt3::gui::PreferencesEditor(parent),
        _ui(new Ui::PreferencesEditor)
{
    tt3::util::ResourceReader rr(Component::Resources::instance(), RSID(PreferencesEditor));

    _ui->setupUi(this);

    //  Set static control values
    _ui->saveIntervalLabel->setText(
        rr.string(RID(SaveIntervalLabel)));

    //  Populate the "save interval" conbo box
    //  Combo box values == int intervals in minutes
    _ui->saveIntervalComboBox->addItem(
        rr.string(RID(SaveInterval1)),
        QVariant::fromValue<int>(1));
    _ui->saveIntervalComboBox->addItem(
        rr.string(RID(SaveInterval2)),
        QVariant::fromValue<int>(2));
    _ui->saveIntervalComboBox->addItem(
        rr.string(RID(SaveInterval3)),
        QVariant::fromValue<int>(3));
    _ui->saveIntervalComboBox->addItem(
        rr.string(RID(SaveInterval5)),
        QVariant::fromValue<int>(5));
    _ui->saveIntervalComboBox->addItem(
        rr.string(RID(SaveInterval10)),
        QVariant::fromValue<int>(10));
    _ui->saveIntervalComboBox->addItem(
        rr.string(RID(SaveInterval15)),
        QVariant::fromValue<int>(15));
    _ui->saveIntervalComboBox->addItem(
        rr.string(RID(SaveInterval30)),
        QVariant::fromValue<int>(30));
    _ui->saveIntervalComboBox->addItem(
        rr.string(RID(SaveInterval60)),
        QVariant::fromValue<int>(60));

    //  Start off with current values from Settings
    loadControlValues();
}

PreferencesEditor::~PreferencesEditor()
{
    delete _ui;
}

//////////
//  PreferencesEditor
auto PreferencesEditor::preferences() const -> Preferences *
{
    return Preferences::instance();
}

void PreferencesEditor::loadControlValues()
{
    _setSelectedSaveInterval(Component::Settings::instance()->saveInterval);
}

void PreferencesEditor::saveControlValues()
{
    Component::Settings::instance()->saveInterval = _selectedSaveInterval();
}

void PreferencesEditor::resetControlValues()
{
    _setSelectedSaveInterval(Component::Settings::instance()->saveInterval.defaultValue());
}

bool PreferencesEditor::isValid() const
{
    return true;
}

//////////
//  Implementation helpers
tt3::util::TimeSpan PreferencesEditor::_selectedSaveInterval() const
{
    return tt3::util::TimeSpan::minutes(_ui->saveIntervalComboBox->currentData().value<int>());
}

void PreferencesEditor::_setSelectedSaveInterval(const tt3::util::TimeSpan & saveInterval)
{
    for (int i = 0; i < _ui->saveIntervalComboBox->count(); i++)
    {
        if (_ui->saveIntervalComboBox->itemData(i).value<int>() == saveInterval.asMinutes())
        {   //  This one!
            _ui->saveIntervalComboBox->setCurrentIndex(i);
        }
    }
}

//////////
//  Signal handlers
void PreferencesEditor::_saveIntervalConboBoxCurrentIndexChanged(int)
{
    emit controlValueChanged();
}

//  End of tt3-db-xml/PreferencesEditor.cpp
