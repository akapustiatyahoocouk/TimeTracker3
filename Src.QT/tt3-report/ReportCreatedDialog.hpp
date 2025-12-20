//
//  tt3-report/ReportCreatedDialog.hpp - The modal "Report created" dialog
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
    namespace Ui { class ReportCreatedDialog; }

    /// \class ReportCreatedDialog tt3-report/API.hpp
    /// \brief The modal "Report created" dialog.
    class TT3_REPORT_PUBLIC ReportCreatedDialog final
        :   private QDialog
    {
        Q_OBJECT
        TT3_CANNOT_ASSIGN_OR_COPY_CONSTRUCT(ReportCreatedDialog)

        //////////
        //  Construction/destruction
    public:
        /// \brief
        ///     Constructs the dialog.
        /// \param parent
        ///     The parent for the dialog; nullptr == none.
        explicit ReportCreatedDialog(
                QWidget * parent,
                const QString & reportFileName
            );

        /// \brief
        ///     The class destructor.
        virtual ~ReportCreatedDialog();

        //////////
        //  Operations
    public:
        /// \brief
        ///     Runs the dialog modally.
        /// \details
        ///     Any choice made by user closes the dialog, but
        ///     may also open the report file while closing the dialog.
        void        doModal();

        //////////
        //  Implementation
    private:
        const QString   _reportFileName;

        //////////
        //  Controls
    private:
        Ui::ReportCreatedDialog *const  _ui;

        //////////
        //  Signal handlers
    private slots:
        void        _openFilePushButtonClicked();
        void        _openLocationPushButtonClicked();
    };
}

//  End of tt3-report/ReportCreatedDialog.hpp
