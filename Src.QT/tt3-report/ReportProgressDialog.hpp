//
//  tt3-report/ReportProgressDialog.hpp - tt3 report progress popup
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
#include "tt3-report/API.hpp"

namespace tt3::report
{
    namespace Ui { class ReportProgressDialog; }

    /// \class ReportProgressDialog tt3-report/API.hpp
    /// \brief The report generation progress popup.
    class TT3_REPORT_PUBLIC ReportProgressDialog final
        :   private QDialog
    {
        Q_OBJECT
        TT3_CANNOT_ASSIGN_OR_COPY_CONSTRUCT(ReportProgressDialog)

        //////////
        //  Constants
    public:
        /// \brief
        ///     The preferred time, in milliseconds, taken
        ///     by a single stage of report generation/
        static const int PreferredStageDurationMs = 5000;

        //////////
        //  Construction/destruction
    public:
        /// \brief
        ///     Constructs the dialog.
        /// \param parent
        ///     The parent for the dialog, nullptr == none.
        explicit ReportProgressDialog(
                QWidget * parent,
                IReportType * reportType,
                const QString & reportDestination
            );

        /// \brief
        ///     The class destructor.
        virtual ~ReportProgressDialog();

        //////////
        //  QWidget
    public:
        /// \brief
        ///     Shows this dialog; has no effect if already shown.
        void            show();

        /// \brief
        ///     Hides this dialog; has no effect if already hidden.
        void            hide();

        //////////
        //  Operations
    public:
        void            reportGenerationProgress(float ratioCompleted);
        void            reportSaveProgress(float ratioCompleted);

        //////////
        //  Implementation
    private:
        float           _lastGenerationRatioCompleted = 0.0f;
        float           _lastSaveRatioCompleted = 0.0f;

        //////////
        //  Controls
    private:
        Ui::ReportProgressDialog *const _ui;
    };
}

//  End of tt3-report/ReportProgressDialog.hpp
