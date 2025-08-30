//
//  tt3-gui/ConfirmRestartDialog.cpp - tt3::gui::ConfirmRestartDialog class implementation
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
#include "ui_ConfirmRestartDialog.h"
using namespace tt3::gui;

//////////
//  Construction/destruction
ConfirmRestartDialog::ConfirmRestartDialog(QWidget * parent)
    :   QDialog(parent),
        _ui(new Ui::ConfirmRestartDialog)
{
    _ui->setupUi(this);

    _ui->buttonBox->button(QDialogButtonBox::StandardButton::Yes)->
        setIcon(QIcon(":/tt3-gui/Resources/Images/Actions/OkSmall.png"));
    _ui->buttonBox->button(QDialogButtonBox::StandardButton::No)->
        setIcon(QIcon(":/tt3-gui/Resources/Images/Actions/CancelSmall.png"));

    _ui->assumeYesCheckBox->setChecked(
        !Component::Settings::instance()->confirmRestart);
}

ConfirmRestartDialog::~ConfirmRestartDialog()
{
    delete _ui;
}

//////////
//  Signal handlers
void ConfirmRestartDialog::_accept()
{
    Component::Settings::instance()->confirmRestart =
        !_ui->assumeYesCheckBox->isChecked();
    accept();
}

//  End of tt3-gui/ConfirmRestartDialog.cpp
