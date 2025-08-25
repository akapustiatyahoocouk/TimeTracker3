//
//  tt3-gui/AboutDialog.cpp - tt3::gui::AboutDialog class implementation
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
#include "ui_AboutDialog.h"
using namespace tt3::gui;

//////////
//  Construction/destruction
AboutDialog::AboutDialog(QWidget * parent)
    :   QDialog(parent),
        _ui(new Ui::AboutDialog)
{
    _ui->setupUi(this);

    _ui->buttonBox->button(QDialogButtonBox::StandardButton::Ok)->
        setIcon(QIcon(":/tt3-gui/Resources/Images/Actions/OkSmall.png"));

    _ui->versionLabel->setText(
        "Version " + Component::instance()->version().toString() +
        " (build " + Component::instance()->buildNumber() +
        ")");
}

AboutDialog::~AboutDialog()
{
    delete _ui;
}

//////////
//  Signal handlers
void AboutDialog::_showLicensePushButtonClicked()
{
    ErrorDialog::show(this, "Not yet implemented");
}

void AboutDialog::_showConfigurationPushButtonClicked()
{
    ErrorDialog::show(this, "Not yet implemented");
}

//  End of tt3-gui/AboutDialog.cpp
