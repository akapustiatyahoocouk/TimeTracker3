//
//  tt3-report-worksummary/SelectUsersDialog.cpp - SelectUsersDialog class implementation
//  TODO Localize via Resources
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
#include "tt3-report-worksummary/API.hpp"
#include "ui_SelectUsersDialog.h"
using namespace tt3::report::worksummary;

//////////
//  Construction/destruction
SelectUsersDialog::SelectUsersDialog(
        QWidget * parent,
        tt3::ws::Workspace workspace,
        tt3::ws::ReportCredentials & credentials,
        SelectionMode selectionMode,
        const tt3::ws::Users & selectedUsers
    ) : QDialog(parent),
        //  Implementation
        _workspace(workspace),
        _credentials(credentials),
        _selectionMode(selectionMode),
        _selectedUsers(selectedUsers),
        //  Controls
        _ui(new Ui::SelectUsersDialog)
{
    _ui->setupUi(this);
}

SelectUsersDialog::~SelectUsersDialog()
{
    delete _ui;
}

//  End of tt3-report-worksummary/SelectUsersDialog.cpp
