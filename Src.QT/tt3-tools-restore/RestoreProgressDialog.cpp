//
//  tt3-tools-restore/RestoreProgressDialog.cpp - tt3::tools::restore::RestoreProgressDialog class implementation
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
#include "tt3-tools-restore/API.hpp"
#include "ui_RestoreProgressDialog.h"
using namespace tt3::tools::restore;

//////////
//  Construction/destruction
RestoreProgressDialog::RestoreProgressDialog(
        QWidget * parent,
        const QString & restoreDestination,
        const QString & restoreSource
    ) : QDialog(parent),
        _ui(new Ui::RestoreProgressDialog)
{
    tt3::util::ResourceReader rr(Component::Resources::instance(), RSID(RestoreProgressDialog));

    _ui->setupUi(this);
    Qt::WindowFlags flags = windowFlags();
    flags |= Qt::CustomizeWindowHint;
    flags |= Qt::WindowStaysOnTopHint;
    flags &= ~Qt::WindowSystemMenuHint;
    flags &= ~Qt::WindowMinMaxButtonsHint;
    flags &= ~Qt::WindowTitleHint;
    flags &= ~Qt::WindowCloseButtonHint;
    setWindowFlags(flags);
    setWindowTitle(rr.string(RID(Title)));

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
void RestoreProgressDialog::reportProgress(float ratioCompleted)
{
    ratioCompleted = std::max(0.0f, std::min(1.0f, ratioCompleted));
    //  Update the UI
    _ui->progressBar->setValue(int(ratioCompleted * 100));
    //  Delay is calculate automatically,
    //  100% == PreferredStartupDurationMs
    float deltaRatioCompleted = std::max(0.0f, ratioCompleted - _lastRatioCompleted);
    int delayMs = int(deltaRatioCompleted * PreferredRestoreDurationMs);
    _lastRatioCompleted = ratioCompleted;
    QDateTime resumeAt = QDateTime::currentDateTimeUtc().addMSecs(delayMs);
    do
    {   //  We need at least 1 guaranteed repaint
        QCoreApplication::processEvents();
    }   while (QDateTime::currentDateTimeUtc() < resumeAt);
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
