//
//  tt3-report-worksummary/DataTypes.hpp - Helper data types
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

namespace tt3::report::worksummary
{
    /// \brief
    ///     Specifies whose works to include into report.
    enum class Scope
    {
        CurrentUser,    ///< Report on the current user.
        SingleUser,     ///< Report on an arbitrary user.
        MultipleUsers   ///< Report on arbitrary set of users.
    };

    /// \brief
    ///     Soecifies how report from/to dates are selected.
    enum class DateRange
    {
        Today,          ///< Today only.
        Yesterday,      ///< Yesterday only.
        LastWeek,       ///< Last week only. Mon-Sun.
        CurrentWeek,    ///< Current week. Mon-Sun.
        CurrentMonth,   ///< Current month.
        CurrentYear,    ///< Current year.
        WeekToDate,     ///< From beginning ot the week until today.
        MonthToDate,    ///< From beginning ot the moth until today.
        YearToDate,     ///< From beginning ot the year until today.
        Custom          ///< From/to dates.
    };

    /// \brief
    ///     Specifies how to group works wothin the report.
    enum class Grouping
    {
        ByActivityType, ///< Group works by activity types.
        ByActivity      ///< Group works by activities.
    };

}

//////////
//  Formatting/parsing
namespace tt3::util
{
    template <> TT3_REPORT_WORKSUMMARY_PUBLIC
    QString toString<tt3::report::worksummary::Scope>
        (const tt3::report::worksummary::Scope & value);
    template <> TT3_REPORT_WORKSUMMARY_PUBLIC
    QString toString<tt3::report::worksummary::DateRange>
        (const tt3::report::worksummary::DateRange & value);
    template <> TT3_REPORT_WORKSUMMARY_PUBLIC
    QString toString<tt3::report::worksummary::Grouping>
        (const tt3::report::worksummary::Grouping & value);

    template <> TT3_REPORT_WORKSUMMARY_PUBLIC
    tt3::report::worksummary::Scope
    fromString<tt3::report::worksummary::Scope>
        (const QString & s, qsizetype & scan);
    template <> TT3_REPORT_WORKSUMMARY_PUBLIC
    tt3::report::worksummary::DateRange
    fromString<tt3::report::worksummary::DateRange>
        (const QString & s, qsizetype & scan);
    template <> TT3_REPORT_WORKSUMMARY_PUBLIC
    tt3::report::worksummary::Grouping
    fromString<tt3::report::worksummary::Grouping>
        (const QString & s, qsizetype & scan);
}

//  End of tt3-report-worksummary/DataTypes.hpp
