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
//  Construction/destruction#include "ShowLicenseDialog.hpp"
ShowLicenseDialog::ShowLicenseDialog(QWidget * parent, tt3::util::ILicense * license)
    :   QDialog(parent),
        _ui(new Ui::ShowLicenseDialog)
{
    _ui->setupUi(this);

    Q_ASSERT(license != nullptr);

    this->setWindowTitle(license->displayName());
    this->setWindowIcon(license->largeIcon());
    _ui->contentTextEdit->setHtml(license->contentAsHtml());
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
