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

    //  Done
    refresh();
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
//  Operations
void MainFrame::refresh()
{
    tt3::ws::WorkspacePtr currentWorkspace = tt3::ws::theCurrentWorkspace;
    //  Frame title
    QString title = "TimeTracker3";
    if (currentWorkspace != nullptr)
    {
        title += " - ";
        title += currentWorkspace->address().displayForm();
    }
    this->setWindowTitle(title);

    //  Menu items
    _ui->actionCloseWorkspace->setEnabled(tt3::ws::theCurrentWorkspace != nullptr);

    //  TODO controls
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

    //  If the workspaceAddress refers to the currently
    //  open workspace, the call is an error
    //  TODO

    //  Create & use
    try
    {
        tt3::ws::WorkspacePtr workspacePtr
            { workspaceAddress.workspaceType()->createWorkspace(workspaceAddress) };
        tt3::ws::theCurrentWorkspace.swap(workspacePtr);
        refresh();
    }
    catch (const tt3::util::Exception & ex)
    {
        tt3::gui::ErrorDialog::show(this, ex);
    }
    catch (const std::exception & ex)
    {
        tt3::gui::ErrorDialog::show(this, ex);
    }
    catch (...)
    {
        tt3::gui::ErrorDialog::show(this);
    }
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
        tt3::ws::WorkspacePtr workspacePtr
            { workspaceAddress.workspaceType()->openWorkspace(workspaceAddress) };
        tt3::ws::theCurrentWorkspace.swap(workspacePtr);
        refresh();
    }
    catch (const tt3::util::Exception & ex)
    {
        tt3::gui::ErrorDialog::show(this, ex);
    }
    catch (const std::exception & ex)
    {
        tt3::gui::ErrorDialog::show(this, ex);
    }
    catch (...)
    {
        tt3::gui::ErrorDialog::show(this);
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

void MainFrame::_onActionHelpContent()
{
    tt3::gui::ErrorDialog::show(this, "Not yet implemented");
}

void MainFrame::_onActionHelpIndex()
{
    tt3::gui::ErrorDialog::show(this, "Not yet implemented");
}

void MainFrame::_onActionHelpSearch()
{
    tt3::gui::ErrorDialog::show(this, "Not yet implemented");
}

//  End of tt3-skin-admin/MainFrame.cpp
