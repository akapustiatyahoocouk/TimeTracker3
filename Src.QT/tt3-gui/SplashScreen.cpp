//
//  tt3-gui/SplashScreen.cpp - tt3::gui::SplashScreen class implementation
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
#include "ui_SplashScreen.h"
using namespace tt3::gui;

//////////
//  Construction/destruction
SplashScreen::SplashScreen()
    :   QDialog(nullptr),
        _ui(new Ui::SplashScreen)
{
    _ui->setupUi(this);
    Qt::WindowFlags flags = windowFlags();
    flags |= Qt::CustomizeWindowHint;
    flags |= Qt::WindowStaysOnTopHint;
    flags &= ~Qt::WindowSystemMenuHint;
    flags &= ~Qt::WindowTitleHint;
    flags &= ~Qt::WindowCloseButtonHint;
    setWindowFlags(flags);
    setWindowTitle(tt3::util::ProductInformation::applicationName());

    //  Set static control values - piggiback on About
    //  dialog for resources
    tt3::util::ResourceReader rr(Component::Resources::instance(), RSID(AboutDialog));
    _ui->productLabel->setText(
        tt3::util::ProductInformation::applicationDisplayName());
    _ui->versionLabel->setText(
        rr.string(
            RID(VersionLabel),
            tt3::util::ProductInformation::applicationVersion().toString(),
            tt3::util::ProductInformation::applicationBuildNumber(),
            QString(qVersion())));
    _ui->copyrightLabel->setText(
        tt3::util::ProductInformation::applicationCopyright());

    //  Adjust controls
    QFont productFont =  _ui->productLabel->font();
    productFont.setPointSize(productFont.pointSize() * 3);
    productFont.setWeight(QFont::Weight::Bold);
    _ui->productLabel->setFont(productFont);

    QFont versionFont =  _ui->versionLabel->font();
    versionFont.setPointSize(versionFont.pointSize() * 3 / 2);
    _ui->versionLabel->setFont(versionFont);

    _ui->progressBar->setValue(0);

    //  Done - no adjusting size, as this will make it too narrow
}

SplashScreen::~SplashScreen()
{
    delete _ui;
}

//////////
//  Operations
void SplashScreen::showStartupProgress(
        const QString & action,
        const QString & context,
        float ratioCompleted
    )
{
    ratioCompleted = std::max(0.0f, std::min(1.0f, ratioCompleted));
    //  Update the UI
    _ui->actionLabel->setText(action);
    _ui->contextLabel->setText(context);
    _ui->progressBar->setValue(int(ratioCompleted * 100));
    //  Delay is calculate automatically,
    //  100% == PreferredStartupDurationMs
    float deltaRatioCompleted = std::max(0.0f, ratioCompleted - _lastRatioCompleted);
    int delayMs = int(deltaRatioCompleted * PreferredStartupDurationMs) / 2;   //  2 stages: discover/init
    _lastRatioCompleted = ratioCompleted;
    QDateTime resumeAt = QDateTime::currentDateTimeUtc().addMSecs(delayMs);
    do
    {   //  We need at least 1 guaranteed repaint
        QCoreApplication::processEvents();
    }   while (QDateTime::currentDateTimeUtc() < resumeAt);
}

//////////
//  Signal handlers
void SplashScreen::accept()
{   //  Don't close on Enter
}

void SplashScreen::reject()
{   //  Don't close on Esc
}

//  End of tt3-gui/SplashScreen.cpp
