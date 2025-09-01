//
//  tt3-gui/ChooseReloginDialog.cpp - tt3::gui::ChooseReloginDialog class implementation
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
#include "ui_ChooseReloginDialog.h"
using namespace tt3::gui;

//////////
//  Construction/destruction
ChooseReloginDialog::ChooseReloginDialog(QWidget * parent, tt3::ws::WorkspaceAddress address)
    :   QDialog(parent),
        _ui(new Ui::ChooseReloginDialog)
{
    _ui->setupUi(this);

    _ui->buttonBox->button(QDialogButtonBox::StandardButton::Yes)->
        setIcon(QIcon(":/tt3-gui/Resources/Images/Actions/OkSmall.png"));
    _ui->buttonBox->button(QDialogButtonBox::StandardButton::No)->
        setIcon(QIcon(":/tt3-gui/Resources/Images/Actions/CancelSmall.png"));

    _ui->promptLabel2->setText(address->displayForm());

    //  Done
    adjustSize();
}

ChooseReloginDialog::~ChooseReloginDialog()
{
    delete _ui;
}

//////////
//  Operations
ChooseReloginDialog::Result ChooseReloginDialog::doModal()
{
    return Result(this->exec());
}

//////////
//  Signal handlers
void ChooseReloginDialog::accept()
{
    done(int(Result::Yes));
}

void ChooseReloginDialog::reject()
{
    done(int(Result::No));
}


//  End of tt3-gui/ChooseReloginDialog.cpp
