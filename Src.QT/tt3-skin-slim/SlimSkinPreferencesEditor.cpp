//
//  tt3-skin-slim/SlimSkinPreferencesEditor.cpp - tt3::skin::slim::SlimSkinPreferencesEditor class implementation
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
    tt3::util::ResourceReader rr(Component::Resources::instance(), RSID(SlimSkinPreferencesEditor));

    _ui->setupUi(this);

    //  Set static control values
    _ui->alwaysOnTopCheckBox->setText(
        rr.string(RID(AlwaysOnTopCheckBox)));
    _ui->opacityLabel->setText(
        rr.string(RID(OpacityLabel)));

    //  Adjust controls
    _ui->opacitySlider->setMinimum(MainFrame::MinOpacity);
    _ui->opacitySlider->setMaximum(MainFrame::MaxOpacity);

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
    _ui->opacitySlider->setValue(
        Component::Settings::instance()->mainFrameOpacity);
}

void SlimSkinPreferencesEditor::saveControlValues()
{
    Component::Settings::instance()->mainFrameAlwaysOnTop =
        _ui->alwaysOnTopCheckBox->isChecked();
    Component::Settings::instance()->mainFrameOpacity =
        _ui->opacitySlider->value();
    //  Apply changes
    if (auto mainFrame = Skin::instance()->_mainFrame)
    {
        mainFrame->setAlwaysOnTop(
            _ui->alwaysOnTopCheckBox->isChecked());
        mainFrame->setOpacity(
            _ui->opacitySlider->value());
    }
}

void SlimSkinPreferencesEditor::resetControlValues()
{
    _ui->alwaysOnTopCheckBox->setChecked(
        Component::Settings::instance()->mainFrameAlwaysOnTop.defaultValue());
    _ui->opacitySlider->setValue(
        Component::Settings::instance()->mainFrameOpacity.defaultValue());
}

bool SlimSkinPreferencesEditor::isValid() const
{
    return true;
}

//  End of tt3-skin-slim/SlimSkinPreferencesEditor.cpp
