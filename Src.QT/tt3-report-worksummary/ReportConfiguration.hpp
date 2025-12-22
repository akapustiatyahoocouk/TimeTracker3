//
//  tt3-report-worksummary/ReportConfiguration.hpp - The Work Summary report configuration
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
    /// \class ReportConfiguration tt3-report-worksummary/API.hpp
    /// \brief The "Work Summary" report configuration.
    class TT3_REPORT_WORKSUMMARY_PUBLIC ReportConfiguration final
        :   public virtual tt3::report::IReportConfiguration
    {
        //////////
        //  Types
    public:
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

        //////////
        //  Construction/destruction/assignment
    public:
        /// \brief
        ///     Constructs the default report configuration.
        ReportConfiguration();

        /// \brief
        ///     Constructs the report configuration/
        /// \param scope
        ///     The scope of the report.
        /// \param userOids
        ///     The OIDs of the users to include into the report.
        /// \param dateRange
        ///     Specofoes how report from/to dates are selected.
        /// \param startDate
        ///     The local date to start from, inclusive.
        /// \param endDate
        ///     The local date to end at, inclusive.
        /// \param grouping
        ///     The works grouping mode.
        /// \param includeDailySummaries
        ///     True ro include the by-day section.
        /// \param includeWeeklySummaries
        ///     True ro include the by-week section.
        /// \param includeMonthlySummaries
        ///     True ro include the by-month section.
        /// \param includeYearlySummaries
        ///     True ro include the by-year section.
        ReportConfiguration(
                Scope scope,
                const tt3::ws::Users & users,
                DateRange dateRange,
                const QDate & startDate,
                const QDate & endDate,
                Grouping grouping,
                bool includeDailySummaries,
                bool includeWeeklySummaries,
                bool includeMonthlySummaries,
                bool includeYearlySummaries,
                float houesPerDay,
                Qt::DayOfWeek weekStart
            );

        /// \brief
        ///     The class destructor.
        virtual ~ReportConfiguration();

        //////////
        //  Operations
    public:
        //////////
        //  Implementation
    private:
        Scope           _scope = Scope::CurrentUser;
        tt3::ws::Users  _users;
        DateRange       _dateRange = DateRange::CurrentMonth;
        QDate           _startDate = QDate::currentDate();  //  inclusive, local date
        QDate           _endDate = QDate::currentDate();    //  inclusive, local date
        Grouping        _grouping = Grouping::ByActivity;
        bool            _includeDailySummaries = true;
        bool            _includeWeeklySummaries = true;
        bool            _includeMonthlySummaries = true;
        bool            _includeYearlySummaries = true;
        float           _houesPerDay = 8.0f;
        Qt::DayOfWeek   _weekStart = Qt::DayOfWeek::Monday ;
    };
}

//////////
//  Formatting/parsing
namespace tt3::util
{
    template <> TT3_REPORT_WORKSUMMARY_PUBLIC
    QString toString<tt3::report::worksummary::ReportConfiguration::Scope>
        (const tt3::report::worksummary::ReportConfiguration::Scope & value);
    template <> TT3_REPORT_WORKSUMMARY_PUBLIC
    QString toString<tt3::report::worksummary::ReportConfiguration::DateRange>
        (const tt3::report::worksummary::ReportConfiguration::DateRange & value);
    template <> TT3_REPORT_WORKSUMMARY_PUBLIC
    QString toString<tt3::report::worksummary::ReportConfiguration::Grouping>
        (const tt3::report::worksummary::ReportConfiguration::Grouping & value);

    template <> TT3_REPORT_WORKSUMMARY_PUBLIC
    tt3::report::worksummary::ReportConfiguration::Scope
    fromString<tt3::report::worksummary::ReportConfiguration::Scope>
        (const QString & s, qsizetype & scan);
    template <> TT3_REPORT_WORKSUMMARY_PUBLIC
    tt3::report::worksummary::ReportConfiguration::DateRange
    fromString<tt3::report::worksummary::ReportConfiguration::DateRange>
        (const QString & s, qsizetype & scan);
    template <> TT3_REPORT_WORKSUMMARY_PUBLIC
    tt3::report::worksummary::ReportConfiguration::Grouping
    fromString<tt3::report::worksummary::ReportConfiguration::Grouping>
        (const QString & s, qsizetype & scan);
}

//  End of tt3-report-worksummary/ReportType.hpp
