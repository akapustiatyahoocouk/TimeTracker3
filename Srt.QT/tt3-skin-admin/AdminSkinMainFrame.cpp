//
//  tt3-skin-admin/AdminSkinMainFrame.cpp - AdminSkinMainFrame class implementation
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
#include "tt3-skin-admin/API.hpp"
#include "ui_AdminSkinMainFrame.h"
using namespace skin::admin;

AdminSkinMainFrame::AdminSkinMainFrame(QWidget * parent)
    :   QMainWindow(parent),
        _ui(new Ui::AdminSkinMainFrame),
        _savePositionTimer()
{
    _ui->setupUi(this);
    _loadPosition();

    _savePositionTimer.setSingleShot(true);
    QObject::connect(&_savePositionTimer, &QTimer::timeout,
                     this, &AdminSkinMainFrame::_savePositionTimerTimeout);
    _trackPosition = true;
}

AdminSkinMainFrame::~AdminSkinMainFrame()
{
    delete _ui;
}

//////////
//  QWidget
void AdminSkinMainFrame::moveEvent(QMoveEvent * event)
{
    QMainWindow::moveEvent(event);
    if (_trackPosition)
    {
        _savePositionTimer.start(500);
    }
}

void AdminSkinMainFrame::resizeEvent(QResizeEvent * event)
{
    QMainWindow::resizeEvent(event);
    if (_trackPosition)
    {
        _savePositionTimer.start(500);
    }
}

void AdminSkinMainFrame::closeEvent(QCloseEvent * event)
{
    event->accept();
    _onActionExit();
}

//////////
//  Implementation
void AdminSkinMainFrame::_loadPosition()
{
    this->setGeometry(AdminSkinSettings::instance()->mainFrameBounds);
    if (AdminSkinSettings::instance()->mainFrameMaximized)
    {
        this->showMaximized();
    }
}

void AdminSkinMainFrame::_savePosition()
{
    if (this->isMaximized())
    {
        AdminSkinSettings::instance()->mainFrameMaximized = true;
    }
    else if (!this->isMinimized())
    {
        AdminSkinSettings::instance()->mainFrameBounds = this->geometry();
        AdminSkinSettings::instance()->mainFrameMaximized = false;
    }
}

//////////
//  Signal handlers
void AdminSkinMainFrame::_savePositionTimerTimeout()
{
    _savePosition();
}

void AdminSkinMainFrame::_onActionExit()
{
    QApplication::exit(0);
}

//  End of tt3-skin-admin/AdminSkinMainFrame.cpp
