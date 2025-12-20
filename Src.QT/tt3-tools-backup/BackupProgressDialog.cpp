//
//  tt3-tools-backup/BackupProgressDialog.cpp - tt3::tools::backup::BackupProgressDialog class implementation
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
#include "tt3-tools-backup/API.hpp"
#include "ui_BackupProgressDialog.h"
using namespace tt3::tools::backup;

//////////
//  Construction/destruction
BackupProgressDialog::BackupProgressDialog(
        QWidget * parent,
        const QString & backupSource,
        const QString & backupDestination
    ) : QDialog(parent),
        _ui(new Ui::BackupProgressDialog)
{
    tt3::util::ResourceReader rr(Component::Resources::instance(), RSID(BackupProgressDialog));

    _ui->setupUi(this);
    Qt::WindowFlags flags = windowFlags();
    flags |= Qt::CustomizeWindowHint;
    flags |= Qt::WindowStaysOnTopHint;
    flags &= ~Qt::WindowSystemMenuHint;
    flags &= ~Qt::WindowTitleHint;
    flags &= ~Qt::WindowCloseButtonHint;
    setWindowFlags(flags);
    setWindowTitle(rr.string(RID(Title)));

    //  Set static control values
    _ui->backupLabel->setText(
        rr.string(RID(BackupLabel)));
    _ui->toLabel->setText(
        rr.string(RID(ToLabel)));

    _ui->buttonBox->button(QDialogButtonBox::StandardButton::Cancel)->
        setText(rr.string(RID(CancelPushButton)));
    _ui->buttonBox->button(QDialogButtonBox::StandardButton::Cancel)->
        setIcon(QIcon(":/tt3-tools-backup/Resources/Images/Actions/CancelSmall.png"));

    //  Adjust controls
    _ui->backupSourceLabel->setText(backupSource);
    _ui->backupDestinationLabel->setText(backupDestination);
    _ui->progressBar->setValue(0);

    //  Done
    adjustSize();
}

BackupProgressDialog::~BackupProgressDialog()
{
    delete _ui;
}

//////////
//  Operations
void BackupProgressDialog::reportProgress(float ratioCompleted)
{
    ratioCompleted = std::max(0.0f, std::min(1.0f, ratioCompleted));
    //  Update the UI
    _ui->progressBar->setValue(int(ratioCompleted * 100));
    //  Delay is calculate automatically,
    //  100% == PreferredStartupDurationMs
    float deltaRatioCompleted = std::max(0.0f, ratioCompleted - _lastRatioCompleted);
    int delayMs = int(deltaRatioCompleted * PreferredBackupDurationMs);
    _lastRatioCompleted = ratioCompleted;
    QDateTime resumeAt = QDateTime::currentDateTimeUtc().addMSecs(delayMs);
    do
    {   //  We need at least 1 guaranteed repaint
        QCoreApplication::processEvents();
    }   while (QDateTime::currentDateTimeUtc() < resumeAt);
}

//////////
//  Signal handlers
void BackupProgressDialog::accept()
{   //  Don't close on Enter
}

void BackupProgressDialog::reject()
{   //  Book a cancellation request
    _cancelRequested = true;
}

//  End of tt3-tools-backup/BackupProgressDialog.cpp
