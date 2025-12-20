//
//  tt3-report/ReportCreatedDialog.cpp - tt3::report::ReportCreatedDialog class implementation
//  TODO localize via Resources
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
#include "tt3-report/API.hpp"
#include "ui_ReportCreatedDialog.h"
using namespace tt3::report;

//////////
//  Construction/destruction
ReportCreatedDialog::ReportCreatedDialog(
        QWidget * parent,
        const QString & reportFileName
    ) : QDialog(parent),
        _reportFileName(reportFileName),
        _ui(new Ui::ReportCreatedDialog)
{
    _ui->setupUi(this);

    //  Adjust controls
    _ui->reportFileNameLabel->setText(reportFileName);

    //  Done
    adjustSize();
    _ui->openFilePushButton->setFocus();
}

ReportCreatedDialog::~ReportCreatedDialog()
{
    delete _ui;
}

//////////
//  Operations
void ReportCreatedDialog::doModal()
{
    this->exec();
}

//////////
//  Signal handlers
void ReportCreatedDialog::_openFilePushButtonClicked()
{
    if (!QDesktopServices::openUrl(QUrl::fromLocalFile(_reportFileName)))
    {
        tt3::gui::ErrorDialog::show("Could not open " + _reportFileName);
    }
    accept();
}

void ReportCreatedDialog::_openLocationPushButtonClicked()
{
    QString location = QFileInfo(_reportFileName).path();
    if (!QDesktopServices::openUrl(QUrl::fromLocalFile(location)))
    {
        tt3::gui::ErrorDialog::show("Could not open " + location);
    }
    accept();
}

//  End of tt3-report/ReportCreatedDialog.cpp
