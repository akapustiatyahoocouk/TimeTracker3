//
//  tt3-report-worksummary/ReportGenerator.hpp - The Work Summary report generator
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
    /// \class ReportGenerator tt3-report-worksummary/API.hpp
    /// \brief The "Work Summary" report generator.
    class TT3_REPORT_WORKSUMMARY_PUBLIC ReportGenerator final
    {
        TT3_CANNOT_ASSIGN_OR_COPY_CONSTRUCT(ReportGenerator)

        //////////
        //  Construction/destruction
    public:
        ReportGenerator(
                tt3::ws::Workspace workspace,
                const tt3::ws::ReportCredentials & credentials,
                const ReportConfiguration & configuration,
                const IReportTemplate * reportTemplate,
                IReportType::ProgressListener progressListener
            );

        /// \brief
        ///     The class destructor.
        ~ReportGenerator();

        //////////
        //  Operations
    public:
        /// \brief
        ///     Geneates the report as configured.
        /// \return
        ///     The generated report.
        /// \exception WorkspaceException
        ///     If a data access error occurs.
        /// \exception ReportException
        ///     If a report generation error occurs.
        ///     This specifically includes the case when
        ///     the specified "configuration" is incompatible
        ///     with this report type.
        auto        generateReport() -> Report *;

        //////////
        //  Implementation
    private:
        const tt3::ws::Workspace                    _workspace;
        const tt3::ws::ReportCredentials            _credentials;
        const ReportConfiguration                   _configuration;
        const IReportTemplate *const   _reportTemplate;
        const IReportType::ProgressListener    _progressListener;

        qsizetype   _totalSteps = 0;
        qsizetype   _completedSteps = 0;

        ReportSection *    _bodySection = nullptr;

        //////////
        //  Report model
    private:
        //  Accounts whose Sorks to include into report
        tt3::ws::Accounts   _accounts;

        //  The date ranges reported for - in ascending order
        struct _DateRange
        {
            _DateRange() {} //  invalid values
            explicit _DateRange(const QDate & date) : startDate(date), endDate(date) {}
            _DateRange(const QDate & start, const QDate & end) : startDate(start), endDate(end) {}

            QDate   startDate;  //  local, inclusive
            QDate   endDate;    //  local, inclusive

            bool    includes(const QDate & date) const
            {
                return date >= startDate && date <= endDate;
            }

            QDateTime   start() const
            {
                return QDateTime(startDate, QTime(0, 0, 0, 0));
            }

            QDateTime   end() const
            {
                return QDateTime(endDate, QTime(23, 59, 59, 999));
            }

            QDateTime   startUtc() const
            {
                return start().toUTC();
            }

            QDateTime   endUtc() const
            {
                return end().toUTC();
            }
        };
        using _DateRanges = QList<_DateRange>;

        _DateRanges _dailyRanges;
        _DateRanges _weeklyRanges;
        _DateRanges _monthlyRanges;
        _DateRanges _yearlyRanges;
        _DateRange  _totalRange;

        //  Column definitions
        struct _ColumnImpl
        {
            _ColumnImpl(const QString & nm)
                :   name(nm) {}
            virtual ~_ColumnImpl() = default;

            QString         name;   //  as visible in the report
        };
        using _Column = std::shared_ptr<_ColumnImpl>;
        using _Columns = QList<_Column>;

        struct _ActivityTypeColumnImpl : public _ColumnImpl
        {
            _ActivityTypeColumnImpl(
                    const QString & nm,
                    tt3::ws::ActivityType at
                ) : _ColumnImpl(nm), activityType(at) {}

            tt3::ws::ActivityType   activityType;
        };

        struct _ActivityColumnImpl : public _ColumnImpl
        {
            _ActivityColumnImpl(
                    const QString & nm,
                    tt3::ws::Activity a
                ) : _ColumnImpl(nm), activity(a) {}

            tt3::ws::Activity   activity;   //  nullptr == not assigned
        };

        _Columns    _columns;   //  in display order, "Totls" column not included

        //  Efforts recorded for a period
        struct _EffortKey
        {
            _EffortKey(const _DateRange & dr,
                       _Column col)
                :   dateRange(dr), column(col) {}

            _DateRange  dateRange;
            _Column     column;

        };
        struct _Less
        {
            bool operator()(const _EffortKey & a, const _EffortKey & b) const
            {   //  We need arbitrary strong sort to use it as a QMap key.
                //  Note, that date ranges in a single series never intersect
                return a.dateRange.startDate < b.dateRange.startDate ||
                       (a.dateRange.startDate == b.dateRange.startDate &&
                        a.column.get() < b.column.get());
            }
        };
        std::map<_EffortKey, qint64, _Less> _efforts;   //  values == msecs

        //////////
        //  Helpers
    private:
        void        _analyze(Report * report);
        void        _analyze(ReportSection * section);
        void        _analyze(ReportBlockElement * block);
        void        _analyze(ReportParagraph * paragraph);
        void        _analyze(ReportList * list);
        void        _analyze(ReportTable * table);
        void        _analyze(ReportTableOfContent * toc);
        void        _analyze(ReportSpanElement * span);
        void        _analyze(ReportText * text);
        void        _analyze(ReportPicture * picture);
        void        _analyze(ReportListItem * item);
        void        _analyze(ReportTableCell * cell);
        void        _prepareAccounts();
        void        _prepareDateRanges();
        void        _collectData(
                            const _DateRanges & dateRanges
                        );
        void        _recordWork(
                            const _DateRange & dateRange,
                            tt3::ws::Work work
                        );
        _Column     _getColumn(
                            tt3::ws::ActivityType activityType
                        );
        _Column     _getColumn(
                            tt3::ws::Activity activity
                        );
        void        _recordEffort(
                            const _DateRange & dateRange,
                            _Column column,
                            qint64 durationMs
                        );
        qint64      _getEffort(
                            const _DateRange & dateRange,
                            _Column column
                        );
        void        _generatePreface();
        void        _generateReportTable(
                            const QString & heading,
                            Report * report,
                            const _DateRanges & dateRanges
                        );
        QString     _formatEffort(qint64 effortMs);
    };
}

//  End of tt3-report-worksummary/ReportGenerator.hpp
