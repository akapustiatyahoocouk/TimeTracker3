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
        QWidget * parent,
        tt3::ws::Workspace workspace_,
        const tt3::ws::ReportCredentials & credentials_
    ) : tt3::report::ReportConfigurationEditor(parent, workspace_, credentials_),
        _ui(new Ui::ReportConfigurationEditor)
{
    tt3::util::ResourceReader rr(Component::Resources::instance(), RSID(ReportConfigurationEditor));

    _ui->setupUi(this);

    //  Set static control values
    _ui->scopeLabel->setText(
        rr.string(RID(ScopeLabel)));
    _ui->currentUserRadioButton->setText(
        rr.string(RID(CurrentUserRadioButton)));
    _ui->singleUserRadioButton->setText(
        rr.string(RID(SingleUserRadioButton)));
    _ui->multipleUsersRadioButton->setText(
        rr.string(RID(MultipleUsersRadioButton)));
    _ui->usersLabel->setText(
        rr.string(RID(UsersLabel)));
    _ui->selectUsersPushButton->setText(
        rr.string(RID(SelectUsersPushButton)));
    _ui->dateRangeLabel->setText(
        rr.string(RID(DateRangeLabel)));
    _ui->todayRadioButton->setText(
        rr.string(RID(TodayRadioButton)));
    _ui->yesterdayRadioButton->setText(
        rr.string(RID(YesterdayRadioButton)));
    _ui->lastWeekRadioButton->setText(
        rr.string(RID(LastWeekRadioButton)));
    _ui->currentWeekRadioButton->setText(
        rr.string(RID(CurrentWeekRadioButton)));
    _ui->currentMonthRadioButton->setText(
        rr.string(RID(CurrentMonthRadioButton)));
    _ui->currentYearRadioButton->setText(
        rr.string(RID(CurrentYearRadioButton)));
    _ui->weekToDateRadioButton->setText(
        rr.string(RID(WeekToDateRadioButton)));
    _ui->monthToDateRadioButton->setText(
        rr.string(RID(MonthToDateRadioButton)));
    _ui->yearToDateRadioButton->setText(
        rr.string(RID(YearToDateRadioButton)));
    _ui->customDatesRadioButton->setText(
        rr.string(RID(CustomDatesRadioButton)));
    _ui->groupByLabel->setText(
        rr.string(RID(GroupByLabel)));
    _ui->groupByActivityTypeRadioButton->setText(
        rr.string(RID(GroupByActivityTypeRadioButton)));
    _ui->groupByActivityRadioButton->setText(
        rr.string(RID(GroupByActivityRadioButton)));
    _ui->includeLabel->setText(
        rr.string(RID(IncludeLabel)));
    _ui->dailyDataCheckBox->setText(
        rr.string(RID(DailyDataCheckBox)));
    _ui->weeklyDataCheckBox->setText(
        rr.string(RID(WeeklyDataCheckBox)));
    _ui->monthlyDataCheckBox->setText(
        rr.string(RID(MonthlyDataCheckBox)));
    _ui->yearlyDataCheckBox->setText(
        rr.string(RID(YearlyDataCheckBox)));
    _ui->hoursPerDayLabel->setText(
        rr.string(RID(HoursPerDayLabel)));
    _ui->weekStartLabel->setText(
        rr.string(RID(WeekStartLabel)));

    //  Populate editable controls
#define TT3_ADD_DAY(Day)                                                \
    _ui->weekStartComboBox->addItem(                                    \
        tt3::util::DateTimeManager::displayName(Qt::DayOfWeek::Day),    \
        QVariant::fromValue(Qt::DayOfWeek::Day));
    TT3_ADD_DAY(Monday)
    TT3_ADD_DAY(Tuesday)
    TT3_ADD_DAY(Wednesday)
    TT3_ADD_DAY(Thursday)
    TT3_ADD_DAY(Friday)
    TT3_ADD_DAY(Saturday)
    TT3_ADD_DAY(Sunday)
#undef TT3_ADD_DAY

    //  Adjust controls
    _ui->fromDateEdit->setDisplayFormat(
        QLocale().dateFormat(QLocale::FormatType::ShortFormat));
    _ui->toDateEdit->setDisplayFormat(
        QLocale().dateFormat(QLocale::FormatType::ShortFormat));

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
    _ui->dailyDataCheckBox->setChecked(settings->includeDailyData);
    _ui->weeklyDataCheckBox->setChecked(settings->includeWeeklyData);
    _ui->monthlyDataCheckBox->setChecked(settings->includeMonthlyData);
    _ui->yearlyDataCheckBox->setChecked(settings->includeYearlyData);
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
    settings->includeDailyData =  _ui->dailyDataCheckBox->isChecked();
    settings->includeWeeklyData = _ui->weeklyDataCheckBox->isChecked();
    settings->includeMonthlyData = _ui->monthlyDataCheckBox->isChecked();
    settings->includeYearlyData = _ui->yearlyDataCheckBox->isChecked();
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
    _ui->dailyDataCheckBox->setChecked(settings->includeDailyData.defaultValue());
    _ui->weeklyDataCheckBox->setChecked(settings->includeWeeklyData.defaultValue());
    _ui->monthlyDataCheckBox->setChecked(settings->includeMonthlyData.defaultValue());
    _ui->yearlyDataCheckBox->setChecked(settings->includeYearlyData.defaultValue());
    _ui->hoursPerDayLineEdit->setText(tt3::util::toString(settings->houesPerDay.defaultValue()));
    _setSelectedWeekStart(settings->weekStart.defaultValue());
    _refresh();
}

bool ReportConfigurationEditor::isValid() const
{
    return
        !_users.isEmpty() &&
        (!_ui->customDatesRadioButton->isChecked() ||
         _ui->fromDateEdit->date() <= _ui->toDateEdit->date()) &&
        (_ui->dailyDataCheckBox->isChecked() ||
         _ui->weeklyDataCheckBox->isChecked() ||
         _ui->monthlyDataCheckBox->isChecked() ||
         _ui->yearlyDataCheckBox->isChecked()) &&
        tt3::util::fromString<float>(_ui->hoursPerDayLineEdit->text(), 0) > 0 &&
        tt3::util::fromString<float>(_ui->hoursPerDayLineEdit->text(), 0) <= 24;
}

auto ReportConfigurationEditor::createReportConfiguration(
    ) const -> ReportConfiguration *
{
    if (!isValid())
    {   //  In case of errors, use defaults
        return new ReportConfiguration();
    }
    QDate today = QDateTime::currentDateTime().date();
    QDate thisWeekStart = today;
    while (thisWeekStart.dayOfWeek() != _selectedWeekStart())
    {
        thisWeekStart = thisWeekStart.addDays(-1);
    }
    QDate thisMonthStart(today.year(), today.month(), 1);

    QDate startDate, endDate;
    switch (_selectedDateRange())
    {
        case DateRange::Today:
            startDate = endDate = today;
            break;
        case DateRange::Yesterday:
            startDate = endDate = today.addDays(-1);
            break;
        case DateRange::LastWeek:
            startDate = thisWeekStart.addDays(-7);
            endDate = thisWeekStart.addDays(-1);
            break;
        case DateRange::CurrentWeek:
            startDate = thisWeekStart;
            endDate = today;
            break;
        case DateRange::CurrentMonth:
            startDate = thisMonthStart;
            endDate = today;
            break;
        case DateRange::CurrentYear:
            startDate = QDate(today.year(), 1, 1);
            endDate = today;
            break;
        case DateRange::WeekToDate:
            startDate = today.addDays(-6);
            endDate = today;
            break;
        case DateRange::MonthToDate:
            startDate = today.addMonths(-1).addDays(1);
            endDate = today;
            break;
        case DateRange::YearToDate:
            startDate = today.addYears(-1).addDays(1);
            endDate = today;
            break;
        case DateRange::Custom:
            startDate = _ui->fromDateEdit->date();
            endDate = std::max(startDate, _ui->toDateEdit->date()); //  ...for sanity
            break;
        default:
            Q_ASSERT(false);
            //  Be defensive in release mode
            startDate = endDate = today;
            break;
    }
    return new ReportConfiguration(
        _users,
        startDate,
        endDate,
        _selectedGrouping(),
        _ui->dailyDataCheckBox->isChecked(),
        _ui->weeklyDataCheckBox->isChecked(),
        _ui->monthlyDataCheckBox->isChecked(),
        _ui->yearlyDataCheckBox->isChecked(),
        tt3::util::fromString<float>(_ui->hoursPerDayLineEdit->text()),
        _selectedWeekStart());
}

//////////
//  Implementation helpers
auto ReportConfigurationEditor::_selectedScope(
    ) const -> Scope
{
    if (_ui->currentUserRadioButton->isChecked())
    {
        return Scope::CurrentUser;
    }
    else if (_ui->singleUserRadioButton->isChecked())
    {
        return Scope::SingleUser;
    }
    else if (_ui->multipleUsersRadioButton->isChecked())
    {
        return Scope::MultipleUsers;
    }
    //  Be defensive in release mode
    return Scope::CurrentUser;
}

void ReportConfigurationEditor::_setSelectedScope(
        Scope scope
    )
{
    switch (scope)
    {
        case Scope::CurrentUser:
            _ui->currentUserRadioButton->setChecked(true);
            break;
        case Scope::SingleUser:
            _ui->singleUserRadioButton->setChecked(true);
            break;
        case Scope::MultipleUsers:
            _ui->multipleUsersRadioButton->setChecked(true);
            break;
        default:    //  Be defensive in release mode
            _ui->currentUserRadioButton->setChecked(true);
            break;
    }
}

auto ReportConfigurationEditor::_selectedDateRange(
    ) const -> DateRange
{
    if (_ui->todayRadioButton->isChecked())
    {
        return DateRange::Today;
    }
    else if (_ui->yesterdayRadioButton->isChecked())
    {
        return DateRange::Yesterday;
    }
    else if (_ui->lastWeekRadioButton->isChecked())
    {
        return DateRange::LastWeek;
    }
    else if (_ui->currentWeekRadioButton->isChecked())
    {
        return DateRange::CurrentWeek;
    }
    else if (_ui->currentMonthRadioButton->isChecked())
    {
        return DateRange::CurrentMonth;
    }
    else if (_ui->currentYearRadioButton->isChecked())
    {
        return DateRange::CurrentYear;
    }
    else if (_ui->weekToDateRadioButton->isChecked())
    {
        return DateRange::WeekToDate;
    }
    else if (_ui->monthToDateRadioButton->isChecked())
    {
        return DateRange::MonthToDate;
    }
    else if (_ui->yearToDateRadioButton->isChecked())
    {
        return DateRange::YearToDate;
    }
    else if (_ui->customDatesRadioButton->isChecked())
    {
        return DateRange::Custom;
    }
    //  Be defensive in release mode
    return DateRange::CurrentWeek;
}

void ReportConfigurationEditor::_setSelectedDateRange(
        DateRange dateRange
    )
{
    switch (dateRange)
    {
        case DateRange::Today:
            _ui->todayRadioButton->setChecked(true);
            break;
        case DateRange::Yesterday:
            _ui->yesterdayRadioButton->setChecked(true);
            break;
        case DateRange::LastWeek:
            _ui->lastWeekRadioButton->setChecked(true);
            break;
        case DateRange::CurrentWeek:
            _ui->currentWeekRadioButton->setChecked(true);
            break;
        case DateRange::CurrentMonth:
            _ui->currentMonthRadioButton->setChecked(true);
            break;
        case DateRange::CurrentYear:
            _ui->currentYearRadioButton->setChecked(true);
            break;
        case DateRange::WeekToDate:
            _ui->weekToDateRadioButton->setChecked(true);
            break;
        case DateRange::MonthToDate:
            _ui->monthToDateRadioButton->setChecked(true);
            break;
        case DateRange::YearToDate:
            _ui->yearToDateRadioButton->setChecked(true);
            break;
        case DateRange::Custom:
            _ui->customDatesRadioButton->setChecked(true);
            break;
        default:    //  Be defensive in release mode
            _ui->currentWeekRadioButton->setChecked(true);
            break;
    }
}

auto ReportConfigurationEditor::_selectedGrouping(
    ) const -> Grouping
{
    if (_ui->groupByActivityTypeRadioButton->isChecked())
    {
        return Grouping::ByActivityType;
    }
    else if (_ui->groupByActivityRadioButton->isChecked())
    {
        return Grouping::ByActivity;
    }
    //  Be defensive in release mode
    return Grouping::ByActivityType;
}

void ReportConfigurationEditor::_setSelectedGrouping(
        Grouping grouping
    )
{
    switch (grouping)
    {
        case Grouping::ByActivityType:
            _ui->groupByActivityTypeRadioButton->setChecked(true);
            break;
        case Grouping::ByActivity:
            _ui->groupByActivityRadioButton->setChecked(true);
            break;
        default:    //  Be defensive in release mode
            _ui->groupByActivityTypeRadioButton->setChecked(true);
            break;
    }
}

auto ReportConfigurationEditor::_selectedWeekStart(
    ) const -> Qt::DayOfWeek
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

void ReportConfigurationEditor::_includeCheckBoxStateChanged(int)
{
    _refresh();
    emit controlValueChanged();
}

void ReportConfigurationEditor::_hoursPerDayLineEditTextChanged(QString)
{
    _refresh();
    emit controlValueChanged();
}

//  End of tt3-report-worksummary/ReportConfigurationEditor.cpp
