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

ReportConfiguration::~ReportConfiguration()
{
}

//////////
//  Formatting/parsing
namespace
{
    struct ScopeName
    {
        ReportConfiguration::Scope  scope;
        const char *    name;
    };
    const ScopeName ScopeNames[] =
    {
        { ReportConfiguration::Scope::CurrentUser,   "CurrentUser" },
        { ReportConfiguration::Scope::SingleUser,    "SingleUser" },
        { ReportConfiguration::Scope::MultipleUsers, "MultipleUsers" }
    };

    struct DateRangeName
    {
        ReportConfiguration::DateRange  dateRange;
        const char *    name;
    };
    const DateRangeName DateRangeNames[] =
    {
        { ReportConfiguration::DateRange::Today,        "Today" },
        { ReportConfiguration::DateRange::Yesterday,    "Yesterday" },
        { ReportConfiguration::DateRange::LastWeek,     "LastWeek" },
        { ReportConfiguration::DateRange::CurrentWeek,  "CurrentWeek" },
        { ReportConfiguration::DateRange::CurrentMonth, "CurrentMonth" },
        { ReportConfiguration::DateRange::CurrentYear,  "CurrentYear" },
        { ReportConfiguration::DateRange::WeekToDate,   "WeekToDate" },
        { ReportConfiguration::DateRange::MonthToDate,  "MonthToDate" },
        { ReportConfiguration::DateRange::YearToDate,   "YearToDate" },
        { ReportConfiguration::DateRange::Custom,       "Custom" }
    };

    struct GroupingName
    {
        ReportConfiguration::Grouping   grouping;
        const char *    name;
    };
    const GroupingName GroupingNames[] =
    {
        { ReportConfiguration::Grouping::ByActivityType, "ByActivityType" },
        { ReportConfiguration::Grouping::ByActivity,     "ByActivity" }
    };
}

template <> TT3_REPORT_WORKSUMMARY_PUBLIC
QString tt3::util::toString<tt3::report::worksummary::ReportConfiguration::Scope>
    (const tt3::report::worksummary::ReportConfiguration::Scope & value)
{
    for (const auto & scopeName : ScopeNames)
    {
        if (scopeName.scope == value)
        {
            return scopeName.name;
        }
    }
    //  Be defensive in release mode
    return ScopeNames[0].name;
}

template <> TT3_REPORT_WORKSUMMARY_PUBLIC
QString tt3::util::toString<tt3::report::worksummary::ReportConfiguration::DateRange>
    (const tt3::report::worksummary::ReportConfiguration::DateRange & value)
{
    for (const auto & dateRangeName : DateRangeNames)
    {
        if (dateRangeName.dateRange == value)
        {
            return dateRangeName.name;
        }
    }
    //  Be defensive in release mode
    return DateRangeNames[0].name;
}

template <> TT3_REPORT_WORKSUMMARY_PUBLIC
QString tt3::util::toString<tt3::report::worksummary::ReportConfiguration::Grouping>
    (const tt3::report::worksummary::ReportConfiguration::Grouping & value)
{
    for (const auto & groupingName : GroupingNames)
    {
        if (groupingName.grouping == value)
        {
            return groupingName.name;
        }
    }
    //  Be defensive in release mode
    return GroupingNames[0].name;
}

template <> TT3_REPORT_WORKSUMMARY_PUBLIC
tt3::report::worksummary::ReportConfiguration::Scope
tt3::util::fromString<tt3::report::worksummary::ReportConfiguration::Scope>
    (const QString & s, qsizetype & scan)
{
    if (scan < 0 || scan > s.length())
    {   //  OOPS!
        throw tt3::util::ParseException(s, scan);
    }
    for (const auto & scopeName : ScopeNames)
    {
        if (s.mid(scan, 16).startsWith(scopeName.name))
        {   //  This one!
            scan += strlen(scopeName.name);
            return scopeName.scope;
        }
    }
    //  Give up
    throw tt3::util::ParseException(s, scan);
}

template <> TT3_REPORT_WORKSUMMARY_PUBLIC
tt3::report::worksummary::ReportConfiguration::DateRange
tt3::util::fromString<tt3::report::worksummary::ReportConfiguration::DateRange>
    (const QString & s, qsizetype & scan)
{
    if (scan < 0 || scan > s.length())
    {   //  OOPS!
        throw tt3::util::ParseException(s, scan);
    }
    for (const auto & dateRangeName : DateRangeNames)
    {
        if (s.mid(scan, 16).startsWith(dateRangeName.name))
        {   //  This one!
            scan += strlen(dateRangeName.name);
            return dateRangeName.dateRange;
        }
    }
    //  Give up
    throw tt3::util::ParseException(s, scan);
}

template <> TT3_REPORT_WORKSUMMARY_PUBLIC
tt3::report::worksummary::ReportConfiguration::Grouping
tt3::util::fromString<tt3::report::worksummary::ReportConfiguration::Grouping>
    (const QString & s, qsizetype & scan)
{
    if (scan < 0 || scan > s.length())
    {   //  OOPS!
        throw tt3::util::ParseException(s, scan);
    }
    for (const auto & groupingName : GroupingNames)
    {
        if (s.mid(scan, 16).startsWith(groupingName.name))
        {   //  This one!
            scan += strlen(groupingName.name);
            return groupingName.grouping;
        }
    }
    //  Give up
    throw tt3::util::ParseException(s, scan);
}

//  End of tt3-report-worksummary/ReportConfiguration.cpp
