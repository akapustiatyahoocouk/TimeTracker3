//
//  tt3-report-worksummary/ReportGenerator.cpp - ReportGenerator class implementation
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
//  Construction/destruction
ReportGenerator::ReportGenerator(
        tt3::ws::Workspace workspace,
        const tt3::ws::ReportCredentials & credentials,
        const ReportConfiguration & configuration,
        const IReportTemplate * reportTemplate,
        IReportType::ProgressListener progressListener
    ) : _workspace(workspace),
        _credentials(credentials),
        _configuration(configuration),
        _reportTemplate(reportTemplate),
        _progressListener(progressListener)
{
}

ReportGenerator::~ReportGenerator()
{
}

//////////
//  Operations
auto ReportGenerator::generateReport() -> Report *
{
    tt3::util::ResourceReader rr(Component::Resources::instance(), RSID(ReportGenerator));

    if (_progressListener != nullptr)
    {
        _progressListener(0.0f);
    }

    //  Prepare to [re]generate the report
    _prepareAccounts();
    _prepareDateRanges();
    _columns.clear();   //  will be filled as report is generated
    _totalSteps = 1; //  report totals
    if (_configuration.includeDailyData())
    {
        _totalSteps += _dailyRanges.size();
    }
    if (_configuration.includeWeeklyData())
    {
        _totalSteps += _weeklyRanges.size();
    }
    if (_configuration.includeMonthlyData())
    {
        _totalSteps += _monthlyRanges.size();
    }
    if (_configuration.includeYearlyData())
    {
        _totalSteps += _yearlyRanges.size();
    }
    _completedSteps = 0;

    //  Make sure we destroy Report on exception
    std::unique_ptr<Report> report
        { _reportTemplate->createNewReport() };
    report->setName(rr.string(RID(ReportName)));
    //  Must analyze initial report created by
    //  template - insert titles, find body section, etc.s
    _analyze(report.get());

    //  Go!
    _generatePreface();
    if (_configuration.includeDailyData())
    {
        _collectData(_dailyRanges);
        _generateReportTable(
            rr.string(RID(DailyBreakdownChapter)),
            report.get(),
            _dailyRanges);
    }
    if (_configuration.includeWeeklyData())
    {
        _collectData(_weeklyRanges);
        _generateReportTable(
            rr.string(RID(WeeklyBreakdownChapter)),
            report.get(),
            _weeklyRanges);
    }
    if (_configuration.includeMonthlyData())
    {
        _collectData(_monthlyRanges);
        _generateReportTable(
            rr.string(RID(MonthlyBreakdownChapter)),
            report.get(),
            _monthlyRanges);
    }
    if (_configuration.includeYearlyData())
    {
        _collectData(_yearlyRanges);
        _generateReportTable(
            rr.string(RID(YearlyBreakdownChapter)),
            report.get(),
            _yearlyRanges);
    }

    //  All done - pass Report ownership out
    if (_progressListener != nullptr)
    {
        _progressListener(1.0f);
    }
    return report.release();
}

//////////
//  Implementation helpers
void ReportGenerator::_analyze(
        Report * report
    )
{
    _bodySection = nullptr;
    for (auto section : report->sections())
    {
        _analyze(section);
    }
    //  The "body" section is mandatory...
    if (_bodySection == nullptr)
    {   //  ...so create one if report template defines none
        _bodySection =
            report->createSection(
                "body",
                report->reportTemplate()->sectionStyle(
                    ISectionStyle::BodyStyleName));
    }
}

void ReportGenerator::_analyze(
        ReportSection * section
    )
{
    if (section->style() != nullptr &&
        section->style()->name() == ISectionStyle::BodyStyleName &&
        _bodySection == nullptr)
    {   //  The 1st "body" section is special
        _bodySection = section;
    }
    for (auto child : section->children())
    {
        _analyze(child);
    }
}

void ReportGenerator::_analyze(
        ReportBlockElement * block
    )
{
    if (auto paragraph =
        dynamic_cast<ReportParagraph*>(block))
    {
        _analyze(paragraph);
    }
    else if (auto list =
             dynamic_cast<ReportList*>(block))
    {
        _analyze(list);
    }
    else if (auto table =
             dynamic_cast<ReportTable*>(block))
    {
        _analyze(table);
    }
    else if (auto toc =
             dynamic_cast<ReportTableOfContent*>(block))
    {
        _analyze(toc);
    }
    else
    {   //  OOPS! Can' be!
        Q_ASSERT(false);
    }
}

void ReportGenerator::_analyze(
        ReportParagraph * paragraph
    )
{
    tt3::util::ResourceReader rr(Component::Resources::instance(), RSID(ReportGenerator));

    if (paragraph->style() != nullptr)
    {   //  Some paragraphs are special
        if (paragraph->style()->name() == IParagraphStyle::TitleStyleName)
        {   //  Replace whatever was placed there by template...
            paragraph->clearChildren();
            paragraph->createText(rr.string(RID(ReportName)));
        }
        else if (paragraph->style()->name() == IParagraphStyle::SubtitleStyleName)
        {   //  Replace whatever was placed there by template...
            paragraph->clearChildren();
            QString subtitle =
                rr.string(
                    RID(CreatedOnMessage),
                    QLocale().toString(QDateTime::currentDateTime().date(), QLocale::ShortFormat));
            paragraph->createText(subtitle);
        }
    }

    for (auto child : paragraph->children())
    {
        _analyze(child);
    }
}

void ReportGenerator::_analyze(
        ReportList * list
    )
{
    for (auto item : list->items())
    {
        _analyze(item);
    }
}

void ReportGenerator::_analyze(
        ReportTable * table
    )
{
    for (auto cell : table->cells())
    {
        _analyze(cell);
    }
}

void ReportGenerator::_analyze(
        ReportTableOfContent * /*toc*/
    )
{   //  Nothing further
}

void ReportGenerator::_analyze(
        ReportSpanElement * span
    )
{
    if (auto text =
        dynamic_cast<ReportText*>(span))
    {
        _analyze(text);
    }
    else if (auto picture =
             dynamic_cast<ReportPicture*>(span))
    {
        _analyze(picture);
    }
    else
    {   //  OOPS! Shouldn't happen!
        Q_ASSERT(false);
    }
}

void ReportGenerator::_analyze(
        ReportText * /*text*/
    )
{   //  Nothing further
}

void ReportGenerator::_analyze(
        ReportPicture * /*picture*/
    )
{   //  Nothing furthern

}

void ReportGenerator::_analyze(
        ReportListItem * item
    )
{
    for (auto child : item->children())
    {
        _analyze(child);
    }
}

void ReportGenerator::_analyze(
        ReportTableCell * cell
    )
{
    for (auto child : cell->children())
    {
        _analyze(child);
    }
}

void ReportGenerator::_prepareAccounts()
{
    _accounts.clear();
    for (const auto & user : _configuration.users())
    {
        _accounts += user->accounts(_credentials);  //  may throw
    }
}

void ReportGenerator::_prepareDateRanges()
{
    //  Daily is easy - just record every day
    _dailyRanges.clear();
    for (QDate date = _configuration.startDate();
         date <= _configuration.endDate();
         date = date.addDays(1))
    {
        _dailyRanges.append(_DateRange(date));
    }

    //  For weekly range, we must start at the 1st day of the
    //  week that is <= startDate, and finish at the last day of
    //  the week which is >= endDate
    _weeklyRanges.clear();
    QDate weekStart = _configuration.startDate();
    while (weekStart.dayOfWeek() != _configuration.weekStart())
    {
        weekStart = weekStart.addDays(-1);
    }
    while (weekStart <= _configuration.endDate())
    {
        _DateRange weekRange(
            std::max(weekStart, _configuration.startDate()),
            std::min(weekStart.addDays(6), _configuration.endDate()));
        _weeklyRanges.append(weekRange);
        weekStart = weekStart.addDays(7);
    }

    //  For monthly range, we must start at the 1st day of the
    //  month that is <= startDate, and finish at the last day of
    //  the month which is >= endDate
    _monthlyRanges.clear();
    QDate monthStart(_configuration.startDate().year(),
                     _configuration.startDate().month(),
                     1);
    while (monthStart <= _configuration.endDate())
    {
        _DateRange monthRange(
            std::max(monthStart, _configuration.startDate()),
            std::min(monthStart.addMonths(1).addDays(-1), _configuration.endDate()));
        _monthlyRanges.append(monthRange);
        monthStart = monthStart.addMonths(1);
    }

    //  For yearly range, we must start at the 1st day of the
    //  year that is <= startDate, and finish at the last day of
    //  the year which is >= endDate
    _yearlyRanges.clear();
    QDate yearStart(_configuration.startDate().year(),
                    1,
                    1);
    while (yearStart <= _configuration.endDate())
    {
        _DateRange yearRange(
            std::max(yearStart, _configuration.startDate()),
            std::min(yearStart.addYears(1).addDays(-1), _configuration.endDate()));
        _yearlyRanges.append(yearRange);
        yearStart = yearStart.addYears(1);
    }

    //  Total is all dates
    _totalRange = _DateRange(_configuration.startDate(), _configuration.endDate());
}

void ReportGenerator::_collectData(
        const _DateRanges & dateRanges
    )
{
    _columns.clear();
    _efforts.clear();
    for (const auto & account : std::as_const(_accounts))
    {
        for (const auto & dateRange : dateRanges)
        {   //  Process all works in this date range...
            for (const auto & work :
                 account->works(_credentials, dateRange.startUtc(), dateRange.endUtc()))
            {
                _recordWork(dateRange, work);
            }
            //  ...and mark 1 step completed
            _completedSteps++;
            if (_progressListener != nullptr)
            {
                _progressListener(float(_completedSteps) / float(_totalSteps));
            }
        }
    }

    //  Columns must be sorted by name
    std::sort(
        _columns.begin(),
        _columns.end(),
        [](auto a, auto b)
        {
            return tt3::util::NaturalStringOrder::less(a->name, b->name);
        });
    //  ...except we want the "no activity type" column LAST!
    for (int i = 0; i < _columns.size(); i++)
    {
        if (auto col =
            std::dynamic_pointer_cast<_ActivityTypeColumnImpl>(_columns[i]))
        {
            if (col->activityType == nullptr)
            {   //  This one!
                _columns.remove(i);
                _columns.append(col);
                break;
            }
        }
    }
}

void ReportGenerator::_recordWork(
        const _DateRange & dateRange,
        tt3::ws::Work work
    )
{
    QDateTime from =
        std::max(
            dateRange.start(),
            work->startedAt(_credentials).toLocalTime());
    QDateTime to =
        std::min(
            dateRange.end(),
            work->finishedAt(_credentials).toLocalTime());
    Q_ASSERT(from <= to);
    _Column column;
    switch (_configuration.grouping())
    {
        case Grouping::ByActivityType:
            column = _getColumn(work->activity(_credentials)->activityType(_credentials));
            break;
        case Grouping::ByActivity:
            column = _getColumn(work->activity(_credentials));
            break;
        default:
            Q_ASSERT(false);
            //  Be defensive in release mode
            column = _getColumn(work->activity(_credentials)->activityType(_credentials));
            break;
    }
    //  Prepare & record duration
    qint64 durationMs = from.msecsTo(to);
    _recordEffort(dateRange, column, durationMs);
}

auto ReportGenerator::_getColumn(
        tt3::ws::ActivityType activityType
    ) -> _Column
{
    tt3::util::ResourceReader rr(Component::Resources::instance(), RSID(ReportGenerator));

    //  Will one of the existing columns do ?
    for (const auto & column : std::as_const(_columns))
    {
        if (auto col =
            std::dynamic_pointer_cast<_ActivityTypeColumnImpl>(column))
        {
            if (col->activityType == activityType)
            {   //  Yes!
                return col;
            }
        }
    }
    //  Need a new column
    auto column =
        std::make_shared<_ActivityTypeColumnImpl>(
            (activityType != nullptr) ?
                activityType->displayName(_credentials) :
                rr.string(RID(OtherColumn)),
            activityType);
    _columns.append(column);
    return column;
}

auto ReportGenerator::_getColumn(
        tt3::ws::Activity activity
    ) -> _Column
{   //  Will one of the existing columns do ?
    for (const auto & column : std::as_const(_columns))
    {
        if (auto col =
            std::dynamic_pointer_cast<_ActivityColumnImpl>(column))
        {
            if (col->activity == activity)
            {   //  Yes!
                return col;
            }
        }
    }
    //  Need a new column
    auto column =
        std::make_shared<_ActivityColumnImpl>(
            activity->displayName(_credentials),
            activity);
    _columns.append(column);
    return column;
}

void ReportGenerator::_recordEffort(
        const _DateRange & dateRange,
        _Column column,
        qint64 durationMs
    )
{
    _EffortKey key(dateRange, column);
    if (_efforts.contains(key))
    {
        _efforts[key] += durationMs;
    }
    else
    {
        _efforts[key] = durationMs;
    }
}

qint64 ReportGenerator::_getEffort(
        const _DateRange & dateRange,
        _Column column
    )
{
    _EffortKey key(dateRange, column);
    if (_efforts.contains(key))
    {
        return _efforts[key];
    }
    return 0;
}

void ReportGenerator::_generatePreface()
{
    tt3::util::ResourceReader rr(Component::Resources::instance(), RSID(ReportGenerator));
    auto reportTemplate = _bodySection->report()->reportTemplate();

    _bodySection
        ->createParagraph(
            reportTemplate->paragraphStyle(IParagraphStyle::Heading1StyleName))
        ->createText(rr.string(RID(PrefaceChapter)));

    _bodySection
        ->createParagraph(
            reportTemplate->paragraphStyle(IParagraphStyle::DefaultStyleName))
        ->createText(rr.string(RID(CreatorMessage)));
    auto currentUser =
        _workspace->login(gui::theCurrentCredentials)
                  ->user(gui::theCurrentCredentials);
    _bodySection
        ->createList(
            reportTemplate->listStyle(IListStyle::DefaultStyleName))
        ->createItem()
        ->createParagraph(
            reportTemplate->paragraphStyle(IParagraphStyle::DefaultStyleName))
        ->createText(currentUser->realName(_credentials));

    _bodySection
        ->createParagraph(
            reportTemplate->paragraphStyle(IParagraphStyle::DefaultStyleName))
        ->createText(rr.string(RID(ReportedDatesMessage)));
    QString periodString =
        QLocale().toString(
            _configuration.startDate(),
            QLocale::ShortFormat);
    if (_configuration.endDate() != _configuration.startDate())
    {
        periodString += " .. ";
        periodString +=
            QLocale().toString(
                _configuration.endDate(),
                QLocale::ShortFormat);
    }
    _bodySection
        ->createList(
            reportTemplate->listStyle(IListStyle::DefaultStyleName))
        ->createItem()
        ->createParagraph(
            reportTemplate->paragraphStyle(IParagraphStyle::DefaultStyleName))
        ->createText(periodString);

    _bodySection
        ->createParagraph(
            reportTemplate->paragraphStyle(IParagraphStyle::DefaultStyleName))
        ->createText(rr.string(RID(IncludedUsersMessage)));
    auto usersList =
        _bodySection
            ->createList(
                reportTemplate->listStyle(IListStyle::DefaultStyleName));
    QList<tt3::ws::User> users;
    for (const auto & user : _configuration.users())
    {
        users.append(user);
    }
    std::sort(
        users.begin(),
        users.end(),
        [&](auto a, auto b)
        {
            return tt3::util::NaturalStringOrder::less(
                a->realName(_credentials),
                b->realName(_credentials));
        });
    for (const auto & user : users)
    {
        usersList
            ->createItem()
            ->createParagraph(
                reportTemplate->paragraphStyle(IParagraphStyle::DefaultStyleName))
            ->createText(user->realName(_credentials));
    }
}

void ReportGenerator::_generateReportTable(
        const QString & heading,
        Report * report,
        const _DateRanges & dateRanges
    )
{
    tt3::util::ResourceReader rr(Component::Resources::instance(), RSID(ReportGenerator));

    _bodySection
        ->createParagraph(
            report->reportTemplate()->paragraphStyle(
                IParagraphStyle::Heading1StyleName))
        ->createText(heading);

    auto table =
        _bodySection->createTable(
            report->reportTemplate()->tableStyle(
                ITableStyle::DefaultStyleName));
    //  Headings
    table
        ->createCell(
            0, 0, 1, 1,
            report->reportTemplate()->tableCellStyle(
                ITableCellStyle::HeadingStyleName))
        ->createParagraph()
        ->createText(rr.string(RID(DateColumn)));
    for (int i = 0; i < _columns.size(); i++)
    {
        table
            ->createCell(
                i + 1, 0, 1, 1,
                TypographicSize::cm(3),
                report->reportTemplate()->tableCellStyle(
                    ITableCellStyle::HeadingStyleName))
            ->createParagraph()
            ->createText(_columns[i]->name);
    }
    table
        ->createCell(
            _columns.size() + 1, 0, 1, 1,
            TypographicSize::cm(3),
            report->reportTemplate()->tableCellStyle(
                ITableCellStyle::HeadingStyleName))
        ->createParagraph()
        ->createText(rr.string(RID(TotalColumn)));

    //  Generate data rows
    qint64 totalEffortMs = 0;
    int numDataRows = 0;
    for (int i = 0; i < dateRanges.size(); i++)
    {
        auto dateRange = dateRanges[i];
        //  Skip ampty (no data) rows
        bool skipRow = true;
        for (int j = 0; j < _columns.size(); j++)
        {
            if (_getEffort(dateRange, _columns[j]) > 0)
            {
                skipRow = false;
            }
        }
        if (skipRow)
        {
            continue;
        }
        //  Date(s) cell
        QString dateString;
        if (dateRange.startDate == dateRange.endDate)
        {
            dateString =
                QLocale().toString(dateRange.startDate, QLocale::ShortFormat);
        }
        else
        {
            dateString =
                QLocale().toString(dateRange.startDate, QLocale::ShortFormat) +
                "-" +
                QLocale().toString(dateRange.endDate, QLocale::ShortFormat);
        }
        table
            ->createCell(
                0, numDataRows + 1, 1, 1,
                report->reportTemplate()->tableCellStyle(
                    ITableCellStyle::DefaultStyleName))
            ->createParagraph()
            ->createText(dateString);
        //  Data cells
        qint64 rowEffortMs = 0;
        for (int j = 0; j < _columns.size(); j++)
        {
            auto effortMs = _getEffort(dateRange, _columns[j]);
            rowEffortMs += effortMs;
            totalEffortMs += effortMs;
            table
                ->createCell(
                    j + 1, numDataRows + 1, 1, 1,
                    TypographicSize::cm(3),
                    report->reportTemplate()->tableCellStyle(
                        ITableCellStyle::DefaultStyleName))
                ->createParagraph()
                ->createText(_formatEffort(effortMs));
        }
        //  Total cell
        table
            ->createCell(
                _columns.size() + 1, numDataRows + 1, 1, 1,
                TypographicSize::cm(3),
                report->reportTemplate()->tableCellStyle(
                    ITableCellStyle::DefaultStyleName))
            ->createParagraph()
            ->createText(_formatEffort(rowEffortMs));
        //  Done with this row
        numDataRows++;
    }

    //   Generate totals row
    table
        ->createCell(
            0, numDataRows + 1, 1, 1,
            report->reportTemplate()->tableCellStyle(
                ITableCellStyle::HeadingStyleName))
        ->createParagraph()
        ->createText(rr.string(RID(TotalRow)));
    for (int i = 0; i < _columns.size(); i++)
    {
        qint64 columnEfforsMs = 0;
        for (auto dateRange : dateRanges)
        {
            columnEfforsMs += _getEffort(dateRange, _columns[i]);
        }
        table
            ->createCell(
                i + 1, numDataRows + 1, 1, 1,
                TypographicSize::cm(3),
                report->reportTemplate()->tableCellStyle(
                    ITableCellStyle::HeadingStyleName))
            ->createParagraph()
            ->createText(_formatEffort(columnEfforsMs));
    }
    table
        ->createCell(
            _columns.size() + 1, numDataRows + 1, 1, 1,
            TypographicSize::cm(3),
            report->reportTemplate()->tableCellStyle(
                ITableCellStyle::HeadingStyleName))
        ->createParagraph()
        ->createText(_formatEffort(totalEffortMs));
}

QString ReportGenerator::_formatEffort(qint64 effortMs)
{
    tt3::util::ResourceReader rr(Component::Resources::instance(), RSID(ReportGenerator));

    auto days = (effortMs / (24 * 60 * 60 * 1000));
    auto hours = (effortMs / (60 * 60 * 1000)) % 24;
    auto minutes = (effortMs / (60 * 1000)) % 60;
    auto seconds = (effortMs / 1000) % 60;
    if (days > 0)
    {
        return rr.string(RID(EffortDHMS), days, hours, minutes, seconds);
    }
    else if (hours > 0)
    {
        return rr.string(RID(EffortHMS), hours, minutes, seconds);
    }
    else if (minutes > 0)
    {
        return rr.string(RID(EffortMS), minutes, seconds);
    }
    else if (seconds > 0)
    {
        return rr.string(RID(EffortS), seconds);
    }
    else
    {
        return rr.string(RID(Effort));
    }
}

//  End of tt3-report-worksummary/ReportGenerator.cpp
