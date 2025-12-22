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

    //  Populate editable controls
    _ui->weekStartComboBox->addItem(
        "Monday",
        QVariant::fromValue(Qt::DayOfWeek::Monday));
    _ui->weekStartComboBox->addItem(
        "Tuesday",
        QVariant::fromValue(Qt::DayOfWeek::Tuesday));
    _ui->weekStartComboBox->addItem(
        "Wednesday",
        QVariant::fromValue(Qt::DayOfWeek::Wednesday));
    _ui->weekStartComboBox->addItem(
        "Thursday",
        QVariant::fromValue(Qt::DayOfWeek::Thursday));
    _ui->weekStartComboBox->addItem(
        "Friday",
        QVariant::fromValue(Qt::DayOfWeek::Friday));
    _ui->weekStartComboBox->addItem(
        "Saturday",
        QVariant::fromValue(Qt::DayOfWeek::Saturday));
    _ui->weekStartComboBox->addItem(
        "Sunday",
        QVariant::fromValue(Qt::DayOfWeek::Sunday));

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
    _setSelectedDateRange(settings->reportDateRange);
    _ui->fromDateEdit->setDate(settings->reportFromDate);
    _ui->toDateEdit->setDate(settings->reportToDate);
    _setSelectedGrouping(settings->reportGrouping);
    _ui->dailyDataCheckBox->setChecked(settings->includeDailySummaries);
    _ui->weeklyDataCheckBox->setChecked(settings->includeWeeklySummaries);
    _ui->monthlyDataCheckBox->setChecked(settings->includeMonthlySummaries);
    _ui->yearlyDataCheckBox->setChecked(settings->includeYearlySummaries);
    _ui->hoursPerDayLineEdit->setText(tt3::util::toString(settings->houesPerDay.value()));
    _setSelectedWeekStart(settings->weekStart);
    _refresh();
}

void ReportConfigurationEditor::saveControlValues()
{
    auto settings = Component::Settings::instance();

    settings->reportScope = _selectedScope();
    settings->reportDateRange = _selectedDateRange();
    settings->reportFromDate = _ui->fromDateEdit->date();
    settings->reportToDate = _ui->toDateEdit->date();
    settings->reportGrouping = _selectedGrouping();
    settings->includeDailySummaries =  _ui->dailyDataCheckBox->isChecked();
    settings->includeWeeklySummaries = _ui->weeklyDataCheckBox->isChecked();
    settings->includeMonthlySummaries = _ui->monthlyDataCheckBox->isChecked();
    settings->includeYearlySummaries = _ui->yearlyDataCheckBox->isChecked();
    settings->houesPerDay =
        tt3::util::fromString(
            _ui->hoursPerDayLineEdit->text(),
            settings->houesPerDay.defaultValue());
    settings->weekStart = _selectedWeekStart();
}

void ReportConfigurationEditor::resetControlValues()
{
    auto settings = Component::Settings::instance();

    _setSelectedScope(settings->reportScope.defaultValue());
    _setSelectedDateRange(settings->reportDateRange.defaultValue());
    _ui->fromDateEdit->setDate(settings->reportFromDate.defaultValue());
    _ui->toDateEdit->setDate(settings->reportToDate.defaultValue());
    _setSelectedGrouping(settings->reportGrouping.defaultValue());
    _ui->dailyDataCheckBox->setChecked(settings->includeDailySummaries.defaultValue());
    _ui->weeklyDataCheckBox->setChecked(settings->includeWeeklySummaries.defaultValue());
    _ui->monthlyDataCheckBox->setChecked(settings->includeMonthlySummaries.defaultValue());
    _ui->yearlyDataCheckBox->setChecked(settings->includeYearlySummaries.defaultValue());
    _ui->hoursPerDayLineEdit->setText(tt3::util::toString(settings->houesPerDay.defaultValue()));
    _setSelectedWeekStart(settings->weekStart.defaultValue());
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
    if (_ui->currentUserRadioButton->isChecked())
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

auto ReportConfigurationEditor::_selectedDateRange(
    ) -> ReportConfiguration::DateRange
{
    if (_ui->todayRadioButton->isChecked())
    {
        return ReportConfiguration::DateRange::Today;
    }
    else if (_ui->yesterdayRadioButton->isChecked())
    {
        return ReportConfiguration::DateRange::Yesterday;
    }
    else if (_ui->lastWeekRadioButton->isChecked())
    {
        return ReportConfiguration::DateRange::LastWeek;
    }
    else if (_ui->currentWeekRadioButton->isChecked())
    {
        return ReportConfiguration::DateRange::CurrentWeek;
    }
    else if (_ui->currentMonthRadioButton->isChecked())
    {
        return ReportConfiguration::DateRange::CurrentMonth;
    }
    else if (_ui->currentYearRadioButton->isChecked())
    {
        return ReportConfiguration::DateRange::CurrentYear;
    }
    else if (_ui->weekToDateRadioButton->isChecked())
    {
        return ReportConfiguration::DateRange::WeekToDate;
    }
    else if (_ui->monthToDateRadioButton->isChecked())
    {
        return ReportConfiguration::DateRange::MonthToDate;
    }
    else if (_ui->yearToDateRadioButton->isChecked())
    {
        return ReportConfiguration::DateRange::YearToDate;
    }
    else if (_ui->customDatesRadioButton->isChecked())
    {
        return ReportConfiguration::DateRange::Custom;
    }
    //  Be defensive in release mode
    return ReportConfiguration::DateRange::CurrentWeek;
}

void ReportConfigurationEditor::_setSelectedDateRange(
        ReportConfiguration::DateRange dateRange
    )
{
    switch (dateRange)
    {
        case ReportConfiguration::DateRange::Today:
            _ui->todayRadioButton->setChecked(true);
            break;
        case ReportConfiguration::DateRange::Yesterday:
            _ui->yesterdayRadioButton->setChecked(true);
            break;
        case ReportConfiguration::DateRange::LastWeek:
            _ui->lastWeekRadioButton->setChecked(true);
            break;
        case ReportConfiguration::DateRange::CurrentWeek:
            _ui->currentWeekRadioButton->setChecked(true);
            break;
        case ReportConfiguration::DateRange::CurrentMonth:
            _ui->currentMonthRadioButton->setChecked(true);
            break;
        case ReportConfiguration::DateRange::CurrentYear:
            _ui->currentYearRadioButton->setChecked(true);
            break;
        case ReportConfiguration::DateRange::WeekToDate:
            _ui->weekToDateRadioButton->setChecked(true);
            break;
        case ReportConfiguration::DateRange::MonthToDate:
            _ui->monthToDateRadioButton->setChecked(true);
            break;
        case ReportConfiguration::DateRange::YearToDate:
            _ui->yearToDateRadioButton->setChecked(true);
            break;
        case ReportConfiguration::DateRange::Custom:
            _ui->customDatesRadioButton->setChecked(true);
            break;
        default:    //  Be defensive in release mode
            _ui->currentWeekRadioButton->setChecked(true);
            break;
    }
}

auto ReportConfigurationEditor::_selectedGrouping(
    ) -> ReportConfiguration::Grouping
{
    if (_ui->groupByActivityTypeRadioButton->isChecked())
    {
        return ReportConfiguration::Grouping::ByActivityType;
    }
    else if (_ui->groupByActivityRadioButton->isChecked())
    {
        return ReportConfiguration::Grouping::ByActivity;
    }
    //  Be defensive in release mode
    return ReportConfiguration::Grouping::ByActivityType;
}

void ReportConfigurationEditor::_setSelectedGrouping(
        ReportConfiguration::Grouping grouping
    )
{
    switch (grouping)
    {
    case ReportConfiguration::Grouping::ByActivityType:
        _ui->groupByActivityTypeRadioButton->setChecked(true);
        break;
    case ReportConfiguration::Grouping::ByActivity:
        _ui->groupByActivityRadioButton->setChecked(true);
        break;
    default:    //  Be defensive in release mode
        _ui->groupByActivityTypeRadioButton->setChecked(true);
        break;
    }
}

auto ReportConfigurationEditor::_selectedWeekStart(
    ) -> Qt::DayOfWeek
{
    return _ui->weekStartComboBox->currentData().value<Qt::DayOfWeek>();
}

void ReportConfigurationEditor::_setSelectedWeekStart(
        Qt::DayOfWeek weekStart
    )
{
    for (int i = 0; i < _ui->weekStartComboBox->count(); i++)
    {
        if (weekStart == _ui->weekStartComboBox->itemData(i).value<Qt::DayOfWeek>())
        {   //  This one
            _ui->weekStartComboBox->setCurrentIndex(i);
            break;
        }
    }
}

void ReportConfigurationEditor::_refresh()
{
    //  Update users to include
    try
    {
        if (_ui->currentUserRadioButton->isChecked())
        {   //  Make sure only the current user is chosen
            auto currentAccount = workspace->login(tt3::gui::theCurrentCredentials);
            auto currentUser = currentAccount->user(credentials);
            _users.clear();
            _users.insert(currentUser);
        }
        else if (_ui->singleUserRadioButton->isChecked())
        {   //  Make sure at most 1 user is chosen
            if (_users.size() != 1)
            {   //  OOPS! Replace!
                auto currentAccount = workspace->login(tt3::gui::theCurrentCredentials);
                auto currentUser = currentAccount->user(credentials);
                _users.clear();
                _users.insert(currentUser);
            }
        }
        else
        {
            Q_ASSERT(_ui->multipleUsersRadioButton->isChecked());
        }
    }
    catch (const tt3::util::Exception & ex)
    {   //  OOPS! Log & recover
        qCritical() << ex;
        _users.clear();
    }

    //  Display included users
    QString usersString;
    try
    {
        QStringList userNames =
            tt3::util::transform(
                QList<tt3::ws::User>(_users.cbegin(), _users.cend()),
                [&](const auto & u)
                {
                    return u->realName(credentials);
                });
        userNames.sort();
        usersString =
            userNames.isEmpty() ?
                "-" :
                userNames.join(", ");
    }
    catch (const tt3::util::Exception & ex)
    {   //  OOPS! Log & recover
        qCritical() << ex;
        usersString = ex.errorMessage();
    }
    _ui->userNamesLabel->setText(usersString);

    //  We cannot "Select" a current user
    _ui->selectUsersPushButton->setEnabled(!_ui->currentUserRadioButton->isChecked());

    //  From/To dates can only be selected in Custom range
    _ui->fromDateEdit->setEnabled(_ui->customDatesRadioButton->isChecked());
    _ui->toDateEdit->setEnabled(_ui->customDatesRadioButton->isChecked());

    //  TODO finish the implementation
}

//////////
//  Signal handlers
void ReportConfigurationEditor::_scopeRadioButtonClicked()
{
    _refresh();
    emit controlValueChanged();
}

void ReportConfigurationEditor::_selectUsersPushButtonClicked()
{
    SelectUsersDialog dlg(
        this,
        workspace,
        credentials,
        _ui->singleUserRadioButton->isChecked() ?
            SelectUsersDialog::SelectionMode::SingleUser :
            SelectUsersDialog::SelectionMode::MultipleUsers,
        _users);
    if (dlg.doModal() == SelectUsersDialog::Result::Ok)
    {   //  Change users selection
        _users = dlg.selectedUsers();
        _refresh();
        emit controlValueChanged();
    }
}

void ReportConfigurationEditor::_dateRangeRadioButtonClicked()
{
    _refresh();
    emit controlValueChanged();
}

void ReportConfigurationEditor::_fromToDateChanged(QDate)
{
    _refresh();
    emit controlValueChanged();
}

//  End of tt3-report-worksummary/ReportConfigurationEditor.cpp
