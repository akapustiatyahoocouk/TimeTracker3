//
//  tt3-report-worksummary/ReportConfigurationEditor.cpp - ReportConfigurationEditor class implementation
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
#include "ui_ReportConfigurationEditor.h"
using namespace tt3::report::worksummary;

//////////
//  Construction/destruction
ReportConfigurationEditor::ReportConfigurationEditor(
        QWidget * parent,
        tt3::ws::Workspace workspace,
        const tt3::ws::ReportCredentials & credentials
    ) : tt3::report::ReportConfigurationEditor(parent, workspace, credentials),
        _ui(new Ui::ReportConfigurationEditor)
{
    _ui->setupUi(this);

    //  Done
    resetControlValues();
}

ReportConfigurationEditor::~ReportConfigurationEditor()
{
    delete _ui;
}

//////////
//  tt3::report::ReportConfigurationEditor
void ReportConfigurationEditor::loadControlValues()
{
    auto settings = Component::Settings::instance();

    _setSelectedScope(settings->reportScope);
    //  TODO implement properly
    _refresh();
}

void ReportConfigurationEditor::saveControlValues()
{
    auto settings = Component::Settings::instance();

    settings->reportScope = _selectedScope();
    //  TODO implement properly
}

void ReportConfigurationEditor::resetControlValues()
{
    auto settings = Component::Settings::instance();

    _setSelectedScope(settings->reportScope.defaultValue());

    //  TODO implement properly
    _refresh();
}

bool ReportConfigurationEditor::isValid() const
{
    return true;    //  TODO implement properly
}

auto ReportConfigurationEditor::createReportConfiguration(
    ) const -> ReportConfiguration *
{
    //  TODO implement properly
    return new ReportConfiguration();
}

//////////
//  Implementation helpers
auto ReportConfigurationEditor::_selectedScope(
    ) -> ReportConfiguration::Scope
{
    if (_ui->currentMonthRadioButton->isChecked())
    {
        return ReportConfiguration::Scope::CurrentUser;
    }
    else if (_ui->singleUserRadioButton->isChecked())
    {
        return ReportConfiguration::Scope::SingleUser;
    }
    else if (_ui->multipleUsersRadioButton->isChecked())
    {
        return ReportConfiguration::Scope::MultipleUsers;
    }
    //  Be defensive in release mode
    return ReportConfiguration::Scope::CurrentUser;
}

void ReportConfigurationEditor::_setSelectedScope(
        ReportConfiguration::Scope scope
    )
{
    switch (scope)
    {
        case ReportConfiguration::Scope::CurrentUser:
            _ui->currentUserRadioButton->setChecked(true);
            break;
        case ReportConfiguration::Scope::SingleUser:
            _ui->singleUserRadioButton->setChecked(true);
            break;
        case ReportConfiguration::Scope::MultipleUsers:
            _ui->multipleUsersRadioButton->setChecked(true);
            break;
        default:    //  Be defensive in release mode
            _ui->currentUserRadioButton->setChecked(true);
            break;
    }
}

void ReportConfigurationEditor::_refresh()
{
}

//  End of tt3-report-worksummary/ReportConfigurationEditor.cpp
