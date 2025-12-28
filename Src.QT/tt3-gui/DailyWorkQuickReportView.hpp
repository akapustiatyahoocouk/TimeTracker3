//
//  tt3-gui/DailyWorkQuickReportView.hpp - The "daily work" quick report view.
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
#pragma once
#include "tt3-gui/API.hpp"
#ifdef TT3_GUI_QUICK_REPORT_VIEW_DEFINED

namespace tt3::gui
{
    namespace Ui { class DailyWorkQuickReportView; }

    /// \class DailyWorkQuickReportView tt3-gui/API.hpp
    /// \brief The "daily work" quick report view.
    class TT3_GUI_PUBLIC DailyWorkQuickReportView final
        :   public QuickReportView
    {
        Q_OBJECT
        TT3_CANNOT_ASSIGN_OR_COPY_CONSTRUCT(DailyWorkQuickReportView)

        //////////
        //  Construction/destruction
    public:
        /// \brief
        ///     Constructs the widget.
        /// \param parent
        ///     The parent for the qwidget; nullptr == none.
        explicit DailyWorkQuickReportView(
                QWidget * parent
            );

        /// \brief
        ///     The class destructor.
        virtual ~DailyWorkQuickReportView();

        //////////
        //  QuickReportView
    public:
        virtual auto    quickReport(
                            ) -> IQuickReport * override;
        virtual void    refresh() override;

        //////////
        //  Implementation
    private:
        bool            _refreshUnderway = false;

        QSet<QColor>    _seedColors;
        QSet<QColor>    _usedPieColors;

        //  Helpers
        void            _clearAndDisableAllControls();
        void            _applyCurrentLocale();
        void            _resetUsedPieColors();
        QColor          _generateUnusedPieColor();
        int             _distance(const QColor & a, const QColor & b);
        QString         _shorten(const QString & s);
        void            _setChart(QChart * chart);

        //  View model
        struct _DayModelImpl;
        struct _ActivityTypeModelImpl;
        struct _ActivityModelImpl;

        using _DayModel = std::shared_ptr<_DayModelImpl>;
        using _ActivityTypeModel = std::shared_ptr<_ActivityTypeModelImpl>;
        using _ActivityModel = std::shared_ptr<_ActivityModelImpl>;

        using _ActivityTypeModels = QList<_ActivityTypeModel>;
        using _ActivityModels = QList<_ActivityModel>;

        struct _DayModelImpl
        {
            _ActivityTypeModels activityTypes;  //  in display order
        };

        struct _ActivityTypeModelImpl
        {
            _ActivityTypeModelImpl(tt3::ws::ActivityType t)
                :   activityType(t) {}
            tt3::ws::ActivityType   activityType;   //  can be nullptr
            _ActivityModels         activities;     //  in display order
            int64_t                 durationMs = 0; //  ...of all relevat Works for the day
        };

        struct _ActivityModelImpl
        {
            _ActivityModelImpl(tt3::ws::Activity a)
                :   activity(a) {}
            tt3::ws::Activity       activity;       //  never nullptr
            int64_t                 durationMs = 0; //  ...of all relevat Works for the day
        };

        _DayModel       _createDayModel(const QDate & date);    //  local date

        //////////
        //  Controls
    private:
        Ui::DailyWorkQuickReportView *const _ui;
        //  Dynamic controls are created at runtime
        QStackedLayout *    _chartPanelLayout;
        QChartView *        _chartView;
        QLabel *            _noDataLabel;
        QLabel *            _errorLabel;
        //  Drawing resoutces
        TreeWidgetDecorations   _decorations;   //  piggyback on it for colors
        LabelDecorations    _labelDecorations;

        //////////
        //  Signal handlers
    private slots:
        void            _currentThemeChanged(ITheme*, ITheme*);
        void            _currentLocaleChanged(QLocale, QLocale);
        void            _scaleSliderValueChanged(int);
        void            _dateRatioButtonClicked();
        void            _dateEditDateChanged(QDate);
        void            _copyPushButtonClicked();
    };
}

#endif  //  def TT3_GUI_QUICK_REPORT_VIEW_DEFINED
//  End of tt3-gui/DailyWorkQuickReportView.hpp

