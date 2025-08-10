//
//  tt3-skin-admin/MainFrame.cpp - tt3::skin::admin::MainFrame class implementation
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
#include "ui_MainFrame.h"
using namespace tt3::skin::admin;

//////////
//  Construction/destruction
MainFrame::MainFrame(QWidget * parent)
    :   QMainWindow(parent),
        _ui(new Ui::MainFrame),
        _savePositionTimer()
{
    _ui->setupUi(this);
    _loadPosition();

    _savePositionTimer.setSingleShot(true);
    QObject::connect(&_savePositionTimer, &QTimer::timeout,
                     this, &MainFrame::_savePositionTimerTimeout);
    _trackPosition = true;
}

MainFrame::~MainFrame()
{
    delete _ui;
}

//////////
//  QWidget
void MainFrame::moveEvent(QMoveEvent * event)
{
    QMainWindow::moveEvent(event);
    if (_trackPosition)
    {
        _savePositionTimer.start(500);
    }
}

void MainFrame::resizeEvent(QResizeEvent * event)
{
    QMainWindow::resizeEvent(event);
    if (_trackPosition)
    {
        _savePositionTimer.start(500);
    }
}

void MainFrame::closeEvent(QCloseEvent * event)
{
    event->accept();
    _onActionExit();
}

//////////
//  Implementation
void MainFrame::_loadPosition()
{
    this->setGeometry(Component::Settings::instance()->mainFrameBounds);
    if (Component::Settings::instance()->mainFrameMaximized)
    {
        this->showMaximized();
    }
}

void MainFrame::_savePosition()
{
    if (this->isMaximized())
    {
        Component::Settings::instance()->mainFrameMaximized = true;
    }
    else if (!this->isMinimized())
    {
        Component::Settings::instance()->mainFrameBounds = this->geometry();
        Component::Settings::instance()->mainFrameMaximized = false;
    }
}

void MainFrame::_createWorkspace(const tt3::ws::WorkspaceAddress & workspaceAddress)
{
    Q_ASSERT(workspaceAddress.isValid());
}

void MainFrame::_openWorkspace(const tt3::ws::WorkspaceAddress & workspaceAddress)
{
    Q_ASSERT(workspaceAddress.isValid());

    //  If the workspaceAddress refers to the currently
    //  open workspace, we don't need to re-open
    //  TODO

    //  Open & use
    try
    {
        std::unique_ptr<tt3::ws::Workspace> workspace
            { workspaceAddress.workspaceType()->openWorkspace(workspaceAddress) };
        //  TODO finish the implementation
    }
    catch (const tt3::ws::WorkspaceException & ex)
    {
        QMessageBox msgBox(this);
        msgBox.setText(ex.errorMessage());
        msgBox.setIcon(QMessageBox::Icon::Critical);
        msgBox.exec();
    }
    catch (...)
    {
        QMessageBox msgBox(this);
        msgBox.setText("Unknown error");
        msgBox.setIcon(QMessageBox::Icon::Critical);
        msgBox.exec();
    }
}

//////////
//  Signal handlers
void MainFrame::_savePositionTimerTimeout()
{
    _savePosition();
}

void MainFrame::_onActionNewWorkspace()
{
    tt3::gui::NewWorkspaceDialog dlg(this);
    if (dlg.exec() == QDialog::Accepted)
    {
        tt3::ws::WorkspaceAddress workspaceAddress = dlg.selectedWorkspaceAddress();
        Q_ASSERT(workspaceAddress.isValid());
        qDebug() << workspaceAddress.displayForm();
        qDebug() << workspaceAddress.externalForm();
        _createWorkspace(workspaceAddress);
    }
}

void MainFrame::_onActionOpenWorkspace()
{
    tt3::gui::OpenWorkspaceDialog dlg(this);
    if (dlg.exec() == QDialog::Accepted)
    {
        tt3::ws::WorkspaceAddress workspaceAddress = dlg.selectedWorkspaceAddress();
        Q_ASSERT(workspaceAddress.isValid());
        qDebug() << workspaceAddress.displayForm();
        qDebug() << workspaceAddress.externalForm();
        _openWorkspace(workspaceAddress);
    }
}

void MainFrame::_onActionExit()
{
    QApplication::exit(0);
}

//  End of tt3-skin-admin/MainFrame.cpp
