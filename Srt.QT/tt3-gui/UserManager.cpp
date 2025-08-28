//
//  tt3-gui/UserManager.cpp - tt3::gui::UserManager class implementation
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
#include "ui_UserManager.h"
using namespace tt3::gui;

//////////
//  Construction/destruction
UserManager::UserManager(QWidget * parent,
                         tt3::ws::IWorkspaceProvider * workspaceProvider,
                         tt3::ws::ICredentialsProvider * credentialsProvider)
    :   QWidget(parent),
        _workspaceProvider(workspaceProvider),
        _credentialsProvider(credentialsProvider),
        _ui(new Ui::UserManager)
{
    Q_ASSERT(_workspaceProvider != nullptr);
    Q_ASSERT(_credentialsProvider != nullptr);

    _ui->setupUi(this);

    //  Set up provider listeners
    connect(&_workspaceProvider->providedWorkspaceNotifier(),
            &tt3::ws::ProvidedWorkspaceNotifier::providedWorkspaceChanged,
            this,
            &UserManager::_onProvidedWorkspaceChanged);
    connect(&_credentialsProvider->providedCredentialsNotifier(),
            &tt3::ws::ProvidedCredentialsNotifier::providedCredentialsChanged,
            this,
            &UserManager::_onProvidedCredentialsChanged);
}

UserManager::~UserManager()
{
    delete _ui;
}

//////////
//  Operaions
void UserManager::refresh()
{
}

//////////
//  Signal handlers
void UserManager::_onProvidedWorkspaceChanged(tt3::ws::Workspace /*before*/, tt3::ws::Workspace /*after*/)
{
    refresh();
}

void UserManager::_onProvidedCredentialsChanged(tt3::ws::Credentials /*before*/, tt3::ws::Credentials /*after*/)
{
    refresh();
}

//  End of tt3-gui/UserManager.cpp
