//
//  tt3-tools-restore/RestoreProgressDialog.cpp - tt3::tools::restore::RestoreProgressDialog class implementation
//  TODO localize via Respurces
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
#include "tt3-tools-restore/API.hpp"
#include "ui_RestoreProgressDialog.h"
using namespace tt3::tools::restore;

//////////
//  Construction/destruction
RestoreProgressDialog::RestoreProgressDialog(
        QWidget * parent,
        const QString & restoreSource,
        const QString & restoreDestination
    ) : QDialog(parent),
        _ui(new Ui::RestoreProgressDialog)
{
    tt3::util::ResourceReader rr(Component::Resources::instance(), RSID(RestoreProgressDialog));

    _ui->setupUi(this);
    Qt::WindowFlags flags = windowFlags();
    flags |= Qt::CustomizeWindowHint;
    flags |= Qt::WindowStaysOnTopHint;
    flags &= ~Qt::WindowSystemMenuHint;
    flags &= ~Qt::WindowTitleHint;
    flags &= ~Qt::WindowCloseButtonHint;
    setWindowFlags(flags);

    //  Set static control values
    _ui->restoreLabel->setText(
        rr.string(RID(RestoreLabel)));
    _ui->fromLabel->setText(
        rr.string(RID(FromLabel)));

    _ui->buttonBox->button(QDialogButtonBox::StandardButton::Cancel)->
        setText(rr.string(RID(CancelPushButton)));
    _ui->buttonBox->button(QDialogButtonBox::StandardButton::Cancel)->
        setIcon(QIcon(":/tt3-tools-restore/Resources/Images/Actions/CancelSmall.png"));

    //  Adjust controls
    _ui->restoreDestinationLabel->setText(restoreDestination);
    _ui->restoreSourceLabel->setText(restoreSource);
    _ui->progressBar->setValue(0);

    //  Done
    adjustSize();
}

RestoreProgressDialog::~RestoreProgressDialog()
{
    delete _ui;
}

//////////
//  Operations
void RestoreProgressDialog::reportProgress(double ratioCompleted)
{
    int newValue = qMax(0, qMin(100, int(ratioCompleted * 100)));
    if (newValue != _ui->progressBar->value())
    {
        _ui->progressBar->setValue(newValue);
        QCoreApplication::processEvents();
    }
}

//////////
//  Signal handlers
void RestoreProgressDialog::accept()
{   //  Don't close on Enter
}

void RestoreProgressDialog::reject()
{   //  Book a cancellation request
    _cancelRequested = true;
}

//  End of tt3-tools-restore/RestoreProgressDialog.cpp
