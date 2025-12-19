//
//  tt3-report/ReportProgressDialog.cpp - tt3::report::ReportProgressDialog class implementation
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
#include "tt3-report/API.hpp"
#include "ui_ReportProgressDialog.h"
using namespace tt3::report;

//////////
//  Construction/destruction
ReportProgressDialog::ReportProgressDialog(
        QWidget * parent,
        IReportType * reportType,
        const QString & reportDestination
    ) : QDialog(parent),
        _ui(new Ui::ReportProgressDialog)
{
    Q_ASSERT(reportType != nullptr);

    tt3::util::ResourceReader rr(Component::Resources::instance(), RSID(ReportProgressDialog));

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
    _ui->generatingLabel->setText(
        rr.string(RID(GeneratingLabel),
                  reportType->displayName()));
    _ui->writingToLabel->setText(
        rr.string(RID(WritingToLabel),
                  reportDestination));

    _ui->buttonBox->button(QDialogButtonBox::StandardButton::Cancel)->
        setText(rr.string(RID(CancelPushButton)));
    _ui->buttonBox->button(QDialogButtonBox::StandardButton::Cancel)->
        setIcon(QIcon(":/tt3-report/Resources/Images/Actions/CancelSmall.png"));

    //  Adjust controls
    _ui->generatingProgressBar->setValue(0);
    _ui->writingToProgressBar->setValue(0);
    _ui->writingToLabel->setEnabled(false);
    _ui->writingToProgressBar->setEnabled(false);

    //  Done
    adjustSize();
}

ReportProgressDialog::~ReportProgressDialog()
{
    delete _ui;
}

//////////
//  QWidget
void ReportProgressDialog::show()
{
    if (!isVisible())
    {
        QDialog::show();
    }
}

void ReportProgressDialog::hide()
{
    if (isVisible())
    {
        QDialog::hide();
    }
}

//////////
//  Operations
void ReportProgressDialog::reportGenerationProgress(float ratioCompleted)
{
    ratioCompleted = std::max(0.0f, std::min(1.0f, ratioCompleted));
    //  Update the UI
    _ui->generatingProgressBar->setValue(int(ratioCompleted * 100));
    //  Delay is calculate automatically,
    //  100% == PreferredStageDurationMs
    float deltaRatioCompleted = std::max(0.0f, ratioCompleted - _lastGenerationRatioCompleted);
    int delayMs = int(deltaRatioCompleted * PreferredStageDurationMs);
    _lastGenerationRatioCompleted = ratioCompleted;
    QDateTime resumeAt = QDateTime::currentDateTimeUtc().addMSecs(delayMs);
    do
    {   //  We need at least 1 guaranteed repaint
        QCoreApplication::processEvents();
    }   while (QDateTime::currentDateTimeUtc() < resumeAt);
}

void ReportProgressDialog::reportSaveProgress(float ratioCompleted)
{
    ratioCompleted = std::max(0.0f, std::min(1.0f, ratioCompleted));
    //  Update the UI
    if (!_ui->writingToLabel->isEnabled())
    {
        _ui->writingToLabel->setEnabled(true);
        _ui->writingToProgressBar->setEnabled(true);
    }
    _ui->writingToProgressBar->setValue(int(ratioCompleted * 100));
    //  Delay is calculate automatically,
    //  100% == PreferredStageDurationMs
    float deltaRatioCompleted = std::max(0.0f, ratioCompleted - _lastSaveRatioCompleted);
    int delayMs = int(deltaRatioCompleted * PreferredStageDurationMs);
    _lastSaveRatioCompleted = ratioCompleted;
    QDateTime resumeAt = QDateTime::currentDateTimeUtc().addMSecs(delayMs);
    do
    {   //  We need at least 1 guaranteed repaint
        QCoreApplication::processEvents();
    }   while (QDateTime::currentDateTimeUtc() < resumeAt);
}

//////////
//  Signal handlers
void ReportProgressDialog::accept()
{   //  Don't close on Enter
}

void ReportProgressDialog::reject()
{   //  Record a cancellation request
    _cancelRequested = true;
}

//  End of tt3-report/ReportProgressDialog.cpp
