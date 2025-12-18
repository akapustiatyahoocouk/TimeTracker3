//
//  tt3-report-worksummary/ReportConfigurationEditor.cpp - ReportConfigurationEditor class implementation
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
#include "tt3-report-worksummary/API.hpp"
#include "ui_ReportConfigurationEditor.h"
using namespace tt3::report::worksummary;

//////////
//  Construction/destruction
ReportConfigurationEditor::ReportConfigurationEditor(
        QWidget * parent
    ) : tt3::report::ReportConfigurationEditor(parent),
        _ui(new Ui::ReportConfigurationEditor)
{
    _ui->setupUi(this);
}

ReportConfigurationEditor::~ReportConfigurationEditor()
{
    delete _ui;
}

//////////
//  tt3::report::ReportConfigurationEditor
void ReportConfigurationEditor::loadControlValues()
{
    //  TODO implement properly
}

void ReportConfigurationEditor::saveControlValues()
{
    //  TODO implement properly
}

void ReportConfigurationEditor::resetControlValues()
{
    //  TODO implement properly
}

bool ReportConfigurationEditor::isValid() const
{
    return true;    //  TODO implement properly
}

//  End of tt3-report-worksummary/ReportConfigurationEditor.cpp
