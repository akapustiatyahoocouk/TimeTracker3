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
}

QuickReportView::~QuickReportView()
{
}

//  End of tt3-gui/QuickReportView.cpp
