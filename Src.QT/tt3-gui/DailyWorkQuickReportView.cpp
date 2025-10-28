//
//  tt3-gui/DailyWorkQuickReportView.cpp - tt3::gui::DailyWorkQuickReportView class implementation
//  TODO translate using UI resources
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
#include "tt3-gui/API.hpp"
#include "ui_DailyWorkQuickReportView.h"
using namespace tt3::gui;

namespace tt3::gui
{
    extern CurrentTheme theCurrentTheme;
    extern CurrentCredentials theCurrentCredentials;
    extern CurrentWorkspace theCurrentWorkspace;
    extern CurrentActivity theCurrentActivity;
}

namespace
{
    class DonutBreakdownMainSlice : public QPieSlice
    {
    public:
        DonutBreakdownMainSlice(QPieSeries * breakdownSeries, QObject * parent)
            :   QPieSlice(parent),
                breakdownSeries(breakdownSeries)
        {
        }

        QPieSeries *const breakdownSeries;
    };

    class DonutBreakdownChart : public QChart
    {
    public:
        DonutBreakdownChart(/*QGraphicsItem * parent, Qt::WindowFlags wFlags*/)
            :   QChart(/*QChart::ChartTypeCartesian, parent, wFlags*/)
        {
            // create the series for main center pie
            _mainSeries = new QPieSeries;
            _mainSeries->setPieSize(0.5);
            QChart::addSeries(_mainSeries);
        }

        void addBreakdownSeries(
                QPieSeries * breakdownSeries,
                const QString & label,
                const QColor & pieColor,
                const QColor & labelColor
            )
        {
            QFont font("Arial", 8);

            //  Add breakdown series as a slice to center pie
            auto mainSlice = new DonutBreakdownMainSlice(breakdownSeries, this);
            mainSlice->setLabel(label);
            mainSlice->setValue(breakdownSeries->sum());
            _mainSeries->append(mainSlice);

            //  Customize the slice
            mainSlice->setBrush(pieColor);
            mainSlice->setLabelVisible();
            mainSlice->setLabelColor(Qt::white);    //  pie slice color is NEVER close to white!
            mainSlice->setLabelPosition(QPieSlice::LabelInsideHorizontal);
            mainSlice->setLabelFont(font);

            //  Position and customize the breakdown series
            breakdownSeries->setPieSize(0.65);
            breakdownSeries->setHoleSize(0.5);
            breakdownSeries->setLabelsVisible();
            QColor sliceColor = pieColor;
            for (QPieSlice * slice : breakdownSeries->slices())
            {
                sliceColor = sliceColor.lighter(115);   //  +15% lightness for each slice
                slice->setBrush(sliceColor);
                slice->setLabelFont(font);
            }

            //  add the series to the chart...
            QChart::addSeries(breakdownSeries);
            //  ...and adjust the chart
            _recalculateAngles();               //  ...for breakdown donut segments
            _updateLegendMarkers(labelColor);   //  ...to customize legend markers
        }

    private:
        QPieSeries *    _mainSeries;

        void _recalculateAngles()
        {
            qreal angle = 0;
            for (QPieSlice * slice : _mainSeries->slices())
            {
                QPieSeries * breakdownSeries = dynamic_cast<DonutBreakdownMainSlice *>(slice)->breakdownSeries;
                breakdownSeries->setPieStartAngle(angle);
                angle += slice->percentage() * 360.0; // full pie is 360.0
                breakdownSeries->setPieEndAngle(angle);
            }
        }

        void _updateLegendMarkers(QBrush textColor)
        {
            for (QAbstractSeries * series : series())
            {   //  Go through all series...
                for (QLegendMarker * marker : legend()->markers(series))
                {   //  ...and markers
                    auto pieMarker = dynamic_cast<QPieLegendMarker *>(marker);
                    if (series == _mainSeries)
                    {   //  ...to modify legend markers from main series...
                        pieMarker->setLabel(pieMarker->slice()->label());
                        pieMarker->setFont(QFont("Arial", 8));
                        pieMarker->setLabelBrush(textColor);
                    }
                    else
                    {   //  ...and hide legend markers from breakdown series
                        pieMarker->setVisible(false);
                    }
                }
            }
        }
    };
}

//////////
//  Construction/destruction
DailyWorkQuickReportView::DailyWorkQuickReportView(QWidget *parent)
    :   QuickReportView(parent),
        //  Implementation
        _seedColors
        {
            QColor(0, 128, 0),
            QColor(128, 0, 128),
            QColor(128, 0, 0),
            QColor(128, 128, 0),
            QColor(128, 0, 128)
        },
        //  Controls
        _ui(new Ui::DailyWorkQuickReportView)
{
    _ui->setupUi(this);

    //  Create dynamic controls
    _chartPanelLayout = new QStackedLayout();
    _chartPanelLayout->setContentsMargins(0, 0, 0, 0);
    _chartView = new QChartView(_ui->chartPanel);
    _chartPanelLayout->addWidget(_chartView);
    _ui->chartPanel->setLayout(_chartPanelLayout);

    //  Set editable control values
    _ui->dateEdit->setDate(QDateTime::currentDateTime().date());    //  TODO from Settings
    _ui->todayRadioButton->setChecked(true);    //  TODO from settings

    //  Theme change means widget decorations change
    connect(&theCurrentTheme,
            &CurrentTheme::changed,
            this,
            &DailyWorkQuickReportView::_currentThemeChanged,
            Qt::ConnectionType::QueuedConnection);

    //  Done
    refresh();
}

DailyWorkQuickReportView::~DailyWorkQuickReportView()
{
    delete _chartView;
    delete _chartPanelLayout;
    delete _ui;
}

//////////
//  QuickReportView
auto DailyWorkQuickReportView::quickReport(
    ) -> IQuickReport * const
{
    return DailyWorkQuickReport::instance();
}

void DailyWorkQuickReportView::refresh()
{
    //  We don't want a refresh() to trigger a recursive refresh()!
    if (auto _ = RefreshGuard(_refreshUnderway)) //  Don't recurse!
    {
        try
        {
            if (workspace() == nullptr || !credentials().isValid() ||
                !workspace()->isOpen() ||
                !workspace()->canAccess(credentials())) //  may throw
            {   //  Nothing to show
                _clearAndDisableAllControls();
                return;
            }
        }
        catch (const tt3::util::Exception & ex)
        {   //  OOPS! No point in proceesing.
            qCritical() << ex;
            _clearAndDisableAllControls();
            return;
        }

        //  Otherwise browser controls are enabled
        _ui->copyPushButton->setEnabled(true);
        _ui->showLabel->setEnabled(true);
        _ui->todayRadioButton->setEnabled(true);
        _ui->forDateRadioButton->setEnabled(true);
        _ui->dateEdit->setEnabled(_ui->forDateRadioButton->isChecked());

        QDate date =
            _ui->forDateRadioButton->isChecked() ?
                _ui->dateEdit->date() :
                QDateTime::currentDateTime().date();
        _DayModel dayModel = _createDayModel(date);

        //  Convert day model to pie chart data
        _resetUsedPieColors();

        auto donutBreakdown = new DonutBreakdownChart();
        donutBreakdown->setAnimationOptions(QChart::NoAnimation);
        donutBreakdown->legend()->setAlignment(Qt::AlignRight);
        for (auto activityTypeModel : dayModel->activityTypes)
        {
            QPieSeries * breakdownSeries = new QPieSeries();
            for (auto activityModel : activityTypeModel->activities)
            {
                int64_t secs = (activityModel->durationMs + 999) / 1000;
                char duration[32];
                sprintf(duration, "[%d:%02d:%02d]\n",
                        int(secs / (60 * 60)),
                        int((secs / 60) % 60),
                        int(secs % 60));
                QString label = "<html><body>";
                label += duration;
                label += "<br>";
                label += _shorten(activityModel->activity->displayName(credentials()).toHtmlEscaped());
                label += "</body></html>";
                QPieSlice * pieSlice = breakdownSeries->append(label, qreal(activityModel->durationMs));
                pieSlice->setLabelBrush(_decorations.itemForeground);
            }
            {
                int64_t secs = (activityTypeModel->durationMs + 999) / 1000;
                char duration[32];
                sprintf(duration, "[%d:%02d:%02d]\n",
                        int(secs / (60 * 60)),
                        int((secs / 60) % 60),
                        int(secs % 60));
                QString label = "<html><body>";
                label += duration;
                label += "<br>";
                if (activityTypeModel->activityType != nullptr)
                {
                    label += _shorten(activityTypeModel->activityType->displayName(credentials()).toHtmlEscaped());
                }
                else
                {
                    label += "-";
                }
                label += "</body></html>";
                donutBreakdown->addBreakdownSeries(
                    breakdownSeries,
                    label,
                    _generateUnusedPieColor(),
                    _decorations.itemForeground.color());
            }
        }

        //  Use the chart we've just created
        _setChart(donutBreakdown);
    }
}

//////////
//  Implementation helpers
void DailyWorkQuickReportView::_clearAndDisableAllControls()
{
    _ui->copyPushButton->setEnabled(false);
    _ui->showLabel->setEnabled(false);
    _ui->todayRadioButton->setEnabled(false);
    _ui->forDateRadioButton->setEnabled(false);
    _ui->dateEdit->setEnabled(false);

    _setChart(nullptr);
}

void DailyWorkQuickReportView::_resetUsedPieColors()
{
    _usedPieColors.clear();
    _usedPieColors.insert(QColor(0, 0, 0));
    _usedPieColors.insert(QColor(0, 0, 255));
    _usedPieColors.insert(QColor(0, 255, 0));
    _usedPieColors.insert(QColor(255, 0, 0));
    _usedPieColors.insert(QColor(255, 255, 255));
}

QColor DailyWorkQuickReportView::_generateUnusedPieColor()
{
    //  Can we use one of the "seed" color ?
    for (QColor c : _seedColors)
    {
        if (!_usedPieColors.contains(c))
        {   //  Use this one!
            _usedPieColors.insert(c);
            return c;
        }
    }
    //  Need to generate a new color, maximally distant from:
    //  1.  Any used seed colors,
    //  2.  Too light or too dark colors.
    int bestDistance = 0;
    QColor bestColor;
    for (int i = 0; i < 1000; i++)
    {
        QColor candidateColor(rand() & 0x7F, rand() & 0x7F, rand() & 0x7F);
        int candidateDistance = INT_MAX;
        for (QColor c : _usedPieColors)
        {
            int d = _distance(candidateColor, c);
            candidateDistance = qMin(candidateDistance, d);
        }
        if (candidateDistance > bestDistance)
        {
            bestDistance = candidateDistance;
            bestColor = candidateColor;
        }
    }
    _usedPieColors.insert(bestColor);
    _seedColors.insert(bestColor);
    return bestColor;
}

int DailyWorkQuickReportView::_distance(const QColor & a, const QColor & b)
{
    int dr = a.red() - b.red();
    int dg = a.green() - b.green();
    int db = a.blue() - b.blue();
    return dr * dr + dg * dg + db * db;
}

QString DailyWorkQuickReportView::_shorten(const QString & s)
{
    return (s.length() <= 20) ? s : (s.left(18) + "...");
}

void DailyWorkQuickReportView::_setChart(QChart * chart)
{
    if (chart == nullptr)
    {   //  Fake an empty chart
        chart = new DonutBreakdownChart();
    }
    chart->setBackgroundBrush(_decorations.background);
    chart->layout()->setContentsMargins(0, 0, 0, 0);
    QChart * oldChart = _chartView->chart();
    _chartView->setChart(chart);
    delete oldChart;
}

//////////
//  View model
auto DailyWorkQuickReportView::_createDayModel(
        const QDate & date
    ) -> _DayModel
{   //  TODO handle errors by creating a "pie chart"
    //  with a single"100% error" message
    tt3::ws::Account clientAccount = workspace()->login(credentials());
    //  Determine the start/end times for the "date"
    QDateTime localDayStart(date, QTime(0, 0));
    QDateTime localDayEnd(date, QTime(23, 59, 59, 999));
    QDateTime utcDayStart = localDayStart.toUTC();
    QDateTime utcDayEnd = localDayEnd.toUTC();
    //  Record relevant Works
    QMap<tt3::ws::Activity, int64_t> activityDurationsMs;
    QMap<tt3::ws::Activity, tt3::ws::ActivityType> activityTypes;
    for (auto work : clientAccount->works(credentials(), utcDayStart, utcDayEnd))
    {
        QDateTime utcWorkStart = qMax(work->startedAt(credentials()), utcDayStart);
        QDateTime utcWorkEnd = qMin(work->finishedAt(credentials()), utcDayEnd);
        Q_ASSERT(utcWorkStart <= utcWorkEnd);
        tt3::ws::Activity activity = work->activity(credentials());
        if (!activityDurationsMs.contains(activity))
        {
            activityDurationsMs[activity] = 0;
        }
        activityDurationsMs[activity] += utcWorkStart.msecsTo(utcWorkEnd);
        activityTypes[activity] = activity->activityType(credentials());
    }
    //  Record "current" activity, if applicable
    if (theCurrentActivity != nullptr)
    {
        QDateTime utcWorkStart = qMax(theCurrentActivity.lastChangedAt(), utcDayStart);
        QDateTime utcWorkEnd = qMin(QDateTime::currentDateTime(), utcDayEnd);
        if (utcWorkStart <= utcWorkEnd)
        {
            if (!activityDurationsMs.contains(theCurrentActivity))
            {
                activityDurationsMs[theCurrentActivity] = 0;
            }
            activityDurationsMs[theCurrentActivity] += utcWorkStart.msecsTo(utcWorkEnd);
            activityTypes[theCurrentActivity] = theCurrentActivity->activityType(credentials());
        }
    }

    //  Build the model
    _DayModel dayModel = std::make_shared<_DayModelImpl>();
    for (tt3::ws::ActivityType activityType :
            tt3::util::unique(activityTypes.values()))
    {
        _ActivityTypeModel activityTypeModel =
            std::make_shared<_ActivityTypeModelImpl>(activityType);
        dayModel->activityTypes.append(activityTypeModel);
        for (auto [activity, durationMs] : activityDurationsMs.asKeyValueRange())
        {
            if (activityTypes[activity] == activityType)
            {
                _ActivityModel activityModel =
                    std::make_shared<_ActivityModelImpl>(activity);
                activityModel->durationMs = durationMs;
                activityTypeModel->activities.append(activityModel);
                activityTypeModel->durationMs += durationMs;
            }
        }
    }
    //  Sort the model(s) in display order
    std::sort(
        dayModel->activityTypes.begin(),
        dayModel->activityTypes.end(),
        [](auto a, auto b)
        {
            return a->durationMs > b->durationMs;
        });
    for (auto activityTypeModel : dayModel->activityTypes)
    {
        std::sort(
            activityTypeModel->activities.begin(),
            activityTypeModel->activities.end(),
            [](auto a, auto b)
            {
                return a->durationMs > b->durationMs;
            });
    }

    //  Done
    return dayModel;
}

//////////
//  Signal handlers
void DailyWorkQuickReportView::_currentThemeChanged(ITheme*, ITheme*)
{
    _decorations = TreeWidgetDecorations();
    refresh();
}

void DailyWorkQuickReportView::_dateRatioButtonClicked()
{
    refresh();
}

void DailyWorkQuickReportView::_dateEditDateChanged(QDate)
{
    refresh();
}

void DailyWorkQuickReportView::_copyPushButtonClicked()
{
    QPixmap pixmap = _chartView->grab();
    QClipboard * clipboard = QApplication::clipboard();
    clipboard->setPixmap(pixmap);
    QMessageBox::information(   //  TODO use dedicated MessageDialog
        this,
        "Image copied",
        "The image has been copied to the system clipboard");
}

//  End of tt3-gui/DailyWorkQuickReportView.cpp
