//
//  tt3-report-worksummary/ReportConfigurationEditor.hpp - tt3-report-worksummary report configuration editor
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
#include "tt3-report-worksummary/API.hpp"

namespace tt3::report::worksummary
{
    namespace Ui { class ReportConfigurationEditor; }

    /// \class ReportConfigurationEditor tt3-report-worksummary/API.hpp
    /// \brief the configuration for a "Wprk Summary" report.
    class TT3_REPORT_WORKSUMMARY_PUBLIC ReportConfigurationEditor final
        :   public tt3::report::ReportConfigurationEditor
    {
        Q_OBJECT
        TT3_CANNOT_ASSIGN_OR_COPY_CONSTRUCT(ReportConfigurationEditor)

        //////////
        //  Construction/destruction
    public:
        /// \brief
        ///     Constructs the report configuration editor.
        /// \param parent
        ///     The parent for the widget; nullptr == none.
        explicit ReportConfigurationEditor(
                QWidget * parent,
                tt3::ws::Workspace workspace,
                const tt3::ws::ReportCredentials & credentials
            );

        /// \brief
        ///     The class destructor.
        virtual ~ReportConfigurationEditor();

        //////////
        //  tt3::report::ReportConfigurationEditor
    public:
        virtual void    loadControlValues() override;
        virtual void    saveControlValues() override;
        virtual void    resetControlValues() override;
        virtual bool    isValid() const override;
        virtual auto    createReportConfiguration(
                            ) const -> ReportConfiguration * override;

        //////////
        //  Implementation
    private:
        tt3::ws::Users  _users; //  yo include into the report

        //  Helpers
        auto            _selectedScope(
                            ) -> ReportConfiguration::Scope;
        void            _setSelectedScope(
                                ReportConfiguration::Scope scope
                            );
        auto            _selectedDateRange(
                            ) -> ReportConfiguration::DateRange;
        void            _setSelectedDateRange(
                                ReportConfiguration::DateRange dateRange
                            );
        auto            _selectedGrouping(
                            ) -> ReportConfiguration::Grouping;
        void            _setSelectedGrouping(
                                ReportConfiguration::Grouping grouping
                            );
        auto            _selectedWeekStart(
                            ) -> Qt::DayOfWeek;
        void            _setSelectedWeekStart(
                                Qt::DayOfWeek weekStart
                            );
        void            _refresh();

        //////////
        //  Controls
    private:
        Ui::ReportConfigurationEditor *const    _ui;

        //////////
        //  Signal handlers
    private slots:
        void            _scopeRadioButtonClicked();
        void            _selectUsersPushButtonClicked();
        void            _dateRangeRadioButtonClicked();
        void            _fromToDateChanged(QDate);
    };
}

//  End of tt3-report-worksummary/ReportConfigurationEditor.hpp
