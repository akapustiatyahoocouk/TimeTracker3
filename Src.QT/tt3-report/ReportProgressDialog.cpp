//
//  tt3-report/ReportProgressDialog.cpp - tt3::report::ReportProgressDialog class implementation
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

    _ui->setupUi(this);

    //  Set static control values
    //  TODO properly
    _ui->generatingLabel->setText(
        "Generating " + reportType->displayName());
    _ui->writingToLabel->setText(
        "Writing to " + reportDestination);

    //  Adjust controls
    _ui->generatingProgressBar->setValue(0);
    _ui->writingToProgressBar->setValue(0);
    _ui->writingToLabel->setVisible(false);
    _ui->writingToProgressBar->setVisible(false);

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
    if (!_ui->writingToLabel->isVisible())
    {
        _ui->writingToLabel->setVisible(true);
        _ui->writingToProgressBar->setVisible(true);
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

//  End of tt3-report/ReportProgressDialog.cpp
