//
//  tt3-gui/QuickReportView.cpp - tt3::gui::QuickReportView class implementation
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
#include "ui_ActivityTypeManager.h"
using namespace tt3::gui;

namespace tt3::gui
{
    extern CurrentCredentials theCurrentCredentials;
    extern CurrentWorkspace theCurrentWorkspace;
}

//////////
//  Construction/destruction
QuickReportView::QuickReportView(
        QWidget * parent
    ) : QWidget(parent),
        _workspace(theCurrentWorkspace),
        _credentials(theCurrentCredentials)
{
    //  Must listen to delayed refresh requests
    connect(this,
            &QuickReportView::refreshRequested,
            this,
            &QuickReportView::_refreshRequested,
            Qt::ConnectionType::QueuedConnection);
}

QuickReportView::~QuickReportView()
{
}

//////////
//  Operaions
tt3::ws::Workspace QuickReportView::workspace() const
{
    return _workspace;
}

void QuickReportView::setWorkspace(tt3::ws::Workspace workspace)
{
    if (workspace != _workspace)
    {   //  We do NOT listen to changes made within the
        //  workspace, as these do NOT cause automatic refresh.
        //  Refreshes are either a) manual or b) on timer.
        _workspace = workspace;
        requestRefresh();
    }
}

tt3::ws::Credentials QuickReportView::credentials() const
{
    return _credentials;
}

void QuickReportView::setCredentials(const tt3::ws::Credentials & credentials)
{
    if (credentials != _credentials)
    {
        _credentials = credentials;
        requestRefresh();
    }
}

void QuickReportView::requestRefresh()
{
    emit refreshRequested();
}

//////////
//  Signal handlers
void QuickReportView::_refreshRequested()
{
    refresh();
}

//  End of tt3-gui/QuickReportView.cpp
