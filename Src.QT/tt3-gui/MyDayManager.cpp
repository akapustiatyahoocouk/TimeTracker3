//
//  tt3-gui/MyDayManager.cpp - tt3::gui::MyDayManager class implementation
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
#include "ui_MyDayManager.h"
using namespace tt3::gui;

namespace tt3::gui
{
    extern CurrentTheme theCurrentTheme;
    extern CurrentCredentials theCurrentCredentials;
    extern CurrentWorkspace theCurrentWorkspace;
}

//////////
//  Construction/destruction
MyDayManager::MyDayManager(
        QWidget * parent
    ) : QWidget(parent),
        //  Implementation
        _workspace(theCurrentWorkspace),
        _credentials(theCurrentCredentials),
        //  Controls
        _ui(new Ui::MyDayManager)
{
    _ui->setupUi(this);

    //  Theme change means widget decorations change
    connect(&theCurrentTheme,
            &CurrentTheme::changed,
            this,
            &MyDayManager::_currentThemeChanged,
            Qt::ConnectionType::QueuedConnection);

    //  Must listen to delayed refresh requests
    connect(this,
            &MyDayManager::refreshRequested,
            this,
            &MyDayManager::_refreshRequested,
            Qt::ConnectionType::QueuedConnection);

    //  Start listening for change notifications
    //  on the currently "viewed" Workspace
    _startListeningToWorkspaceChanges();
}

MyDayManager::~MyDayManager()
{
    _stopListeningToWorkspaceChanges();
    delete _ui;
}

//////////
//  Operaions
tt3::ws::Workspace MyDayManager::workspace() const
{
    return _workspace;
}

void MyDayManager::setWorkspace(tt3::ws::Workspace workspace)
{
    if (workspace != _workspace)
    {
        _stopListeningToWorkspaceChanges();
        _workspace = workspace;
        _startListeningToWorkspaceChanges();
        requestRefresh();
    }
}

tt3::ws::Credentials MyDayManager::credentials() const
{
    return _credentials;
}

void MyDayManager::setCredentials(const tt3::ws::Credentials & credentials)
{
    if (credentials != _credentials)
    {
        _credentials = credentials;
        requestRefresh();
    }
}

void MyDayManager::refresh()
{   //  TODO implement
}

void MyDayManager::requestRefresh()
{
    emit refreshRequested();
}

//////////
//  Implementation helpers
void MyDayManager::_startListeningToWorkspaceChanges()
{
    if (_workspace != nullptr)
    {
        connect(_workspace.get(),
                &tt3::ws::WorkspaceImpl::workspaceClosed,
                this,
                &MyDayManager::_workspaceClosed,
                Qt::ConnectionType::QueuedConnection);
        connect(_workspace.get(),
                &tt3::ws::WorkspaceImpl::objectCreated,
                this,
                &MyDayManager::_objectCreated,
                Qt::ConnectionType::QueuedConnection);
        connect(_workspace.get(),
                &tt3::ws::WorkspaceImpl::objectDestroyed,
                this,
                &MyDayManager::_objectDestroyed,
                Qt::ConnectionType::QueuedConnection);
        connect(_workspace.get(),
                &tt3::ws::WorkspaceImpl::objectModified,
                this,
                &MyDayManager::_objectModified,
                Qt::ConnectionType::QueuedConnection);
    }
}

void MyDayManager::_stopListeningToWorkspaceChanges()
{
    if (_workspace != nullptr)
    {
        disconnect(_workspace.get(),
                   &tt3::ws::WorkspaceImpl::workspaceClosed,
                   this,
                   &MyDayManager::_workspaceClosed);
        disconnect(_workspace.get(),
                   &tt3::ws::WorkspaceImpl::objectCreated,
                   this,
                   &MyDayManager::_objectCreated);
        disconnect(_workspace.get(),
                   &tt3::ws::WorkspaceImpl::objectDestroyed,
                   this,
                   &MyDayManager::_objectDestroyed);
        disconnect(_workspace.get(),
                   &tt3::ws::WorkspaceImpl::objectModified,
                   this,
                   &MyDayManager::_objectModified);
    }
}

void MyDayManager::_clearAndDisableAllControls()
{   //  TODO implement
}

//////////
//  Signal handlers
void MyDayManager::_currentThemeChanged(ITheme *, ITheme *)
{
    requestRefresh();
}

void MyDayManager::_workspaceClosed(tt3::ws::WorkspaceClosedNotification /*notification*/)
{
    requestRefresh();
}

void MyDayManager::_objectCreated(tt3::ws::ObjectCreatedNotification /*notification*/)
{
    requestRefresh();
}

void MyDayManager::_objectDestroyed(tt3::ws::ObjectDestroyedNotification /*notification*/)
{
    requestRefresh();
}

void MyDayManager::_objectModified(tt3::ws::ObjectModifiedNotification /*notification*/)
{
    requestRefresh();
}

void MyDayManager::_refreshRequested()
{
    refresh();
}

//  End of tt3-gui/MyDayManager.cpp
