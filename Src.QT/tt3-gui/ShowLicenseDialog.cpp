//
//  tt3-gui/ShowLicenseDialog.cpp - tt3::gui::ShowLicenseDialog class implementation
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
#include "ui_ShowLicenseDialog.h"
using namespace tt3::gui;

//////////
//  Construction/destruction
ShowLicenseDialog::ShowLicenseDialog(QWidget * parent, tt3::util::ILicense * license)
    :   QDialog(parent),
        _ui(new Ui::ShowLicenseDialog)
{
    tt3::util::ResourceReader rr(Component::Resources::instance(), RSID(ShowLicenseDialog));

    Q_ASSERT(license != nullptr);

    _ui->setupUi(this);
    setWindowTitle(license->displayName());
    setWindowIcon(license->largeIcon());

    _ui->buttonBox->button(QDialogButtonBox::StandardButton::Ok)->
        setText(rr.string(RID(OkPushButton)));
    _ui->buttonBox->button(QDialogButtonBox::StandardButton::Ok)->
        setIcon(QIcon(":/tt3-gui/Resources/Images/Actions/OkSmall.png"));

    _ui->contentTextEdit->setHtml(license->contentAsHtml());

    //  Done
    adjustSize();
}

ShowLicenseDialog::~ShowLicenseDialog()
{
    delete _ui;
}

//////////
//  Operations
void ShowLicenseDialog::doModal()
{
    exec();
}

//  End of tt3-gui/ShowLicenseDialog.cpp
