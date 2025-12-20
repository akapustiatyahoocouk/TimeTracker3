//
//  tt3-report/ReportConfigurationEditor.cpp - tt3::report::ReportConfigurationEditor class implementation
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
#include "tt3-report/API.hpp"
using namespace tt3::report;

//////////
//  Construction/destruction
ReportConfigurationEditor::ReportConfigurationEditor(
        QWidget * parent,
        tt3::ws::Workspace workspace_,
        const tt3::ws::ReportCredentials & credentials_
    ) : QWidget(parent),
        workspace(workspace_),
        credentials(credentials_)
{
}

ReportConfigurationEditor::~ReportConfigurationEditor()
{
}

//  End of tt3-report/ReportConfigurationEditor.cpp
