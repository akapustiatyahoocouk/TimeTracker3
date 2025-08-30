//
//  tt3-gui/ConfirmExitDialog.cpp - tt3::gui::ConfirmExitDialog class implementation
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
#include "ui_ConfirmExitDialog.h"
using namespace tt3::gui;

//////////
//  Construction/destruction
ConfirmExitDialog::ConfirmExitDialog(QWidget * parent)
    :   QDialog(parent),
        _ui(new Ui::ConfirmExitDialog)
{
    _ui->setupUi(this);

    _ui->buttonBox->button(QDialogButtonBox::StandardButton::Yes)->
        setIcon(QIcon(":/tt3-gui/Resources/Images/Actions/OkSmall.png"));
    _ui->buttonBox->button(QDialogButtonBox::StandardButton::No)->
        setIcon(QIcon(":/tt3-gui/Resources/Images/Actions/CancelSmall.png"));

    _ui->assumeYesCheckBox->setChecked(
        !Component::Settings::instance()->confirmExit);
}

ConfirmExitDialog::~ConfirmExitDialog()
{
    delete _ui;
}

//////////
//  Signal handlers
void ConfirmExitDialog::_accept()
{
    Component::Settings::instance()->confirmExit =
        !_ui->assumeYesCheckBox->isChecked();
    accept();
}

//  End of tt3-gui/ConfirmExitDialog.cpp
