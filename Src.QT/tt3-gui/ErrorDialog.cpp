//
//  tt3-gui/ErrorDialog.cpp - tt3::gui::ErrorDialog class implementation (+specializations)
//  TODO translate UI via Resources
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
#include "ui_ErrorDialog.h"
using namespace tt3::gui;

//////////
//  Construction/destruction
ErrorDialog::ErrorDialog(QWidget * parent, const QString & errorMessage)
    :   QDialog(parent),
        _ui(new Ui::ErrorDialog)
{
    _ui->setupUi(this);

    _ui->buttonBox->button(QDialogButtonBox::StandardButton::Ok)->
        setIcon(QIcon(":/tt3-gui/Resources/Images/Actions/OkSmall.png"));

    _ui->messageLabel->setText(errorMessage);

    //  Done
    adjustSize();
}

ErrorDialog::ErrorDialog(QWidget * parent, const tt3::util::Exception & ex)
    :   ErrorDialog(parent, ex.errorMessage()) {}

ErrorDialog::~ErrorDialog()
{
    delete _ui;
}

//////////
//  Operations
void ErrorDialog::doModal()
{
    this->exec();
}

void ErrorDialog::show(const tt3::util::Exception & ex)
{
    ErrorDialog dlg(nullptr, ex);
    dlg.doModal();
}

void ErrorDialog::show(const QString & errorMessage)
{
    ErrorDialog dlg(nullptr, errorMessage);
    dlg.doModal();
}

void ErrorDialog::show(QWidget * parent, const tt3::util::Exception & ex)
{
    ErrorDialog dlg(parent, ex);
    dlg.doModal();
}

void ErrorDialog::show(QWidget * parent, const QString & errorMessage)
{
    ErrorDialog dlg(parent, errorMessage);
    dlg.doModal();
}

//  End of tt3-gui/ErrorDialog.cpp
