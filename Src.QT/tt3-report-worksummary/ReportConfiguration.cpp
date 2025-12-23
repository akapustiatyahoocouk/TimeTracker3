//
//  tt3-report-worksummary/ReportConfiguration.cpp - ReportConfiguration class implementation
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
using namespace tt3::report::worksummary;

//////////
//  Construction/destruction/assignment
ReportConfiguration::ReportConfiguration()
{
}

ReportConfiguration::ReportConfiguration(
        const tt3::ws::Users & users,
        const QDate & startDate,
        const QDate & endDate,
        Grouping grouping,
        bool includeDailyData,
        bool includeWeeklyData,
        bool includeMonthlyData,
        bool includeYearlyData,
        float houesPerDay,
        Qt::DayOfWeek weekStart
    ) : _users(users),
        _startDate(startDate),
        _endDate(endDate),
        _grouping(grouping),
        _includeDailyData(includeDailyData),
        _includeWeeklyData(includeWeeklyData),
        _includeMonthlyData(includeMonthlyData),
        _includeYearlyData(includeYearlyData),
        _houesPerDay(houesPerDay),
        _weekStart(weekStart)
{
}

//  End of tt3-report-worksummary/ReportConfiguration.cpp
