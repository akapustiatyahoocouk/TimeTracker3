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
        :   public virtual IReportConfiguration
    {
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
            );

        //  Default copy-constructor, destructor and
        //  assignment are all OK

        //////////
        //  Operations
    public:
        tt3::ws::Users  users() const { return _users; }
        QDate           startDate() const { return _startDate; }
        QDate           endDate() const { return _endDate; }
        Grouping        grouping() const { return _grouping; }
        bool            includeDailyData() const { return _includeDailyData; }
        bool            includeWeeklyData() const { return _includeWeeklyData; }
        bool            includeMonthlyData() const { return _includeMonthlyData; }
        bool            includeYearlyData() const { return _includeYearlyData; }
        float           houesPerDay() const { return _houesPerDay; }
        Qt::DayOfWeek   weekStart() const { return _weekStart; }

        //////////
        //  Implementation
    private:
        tt3::ws::Users  _users;
        QDate           _startDate = QDate::currentDate();  //  inclusive, local date
        QDate           _endDate = QDate::currentDate();    //  inclusive, local date
        Grouping        _grouping = Grouping::ByActivity;
        bool            _includeDailyData = true;
        bool            _includeWeeklyData = true;
        bool            _includeMonthlyData = true;
        bool            _includeYearlyData = true;
        float           _houesPerDay = 8.0f;
        Qt::DayOfWeek   _weekStart = Qt::DayOfWeek::Monday ;
    };
}

//  End of tt3-report-worksummary/ReportType.hpp
