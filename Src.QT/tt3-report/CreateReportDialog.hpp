//
//  tt3-report/CreateReportDialog.hpp - The modal "Create report" dialog
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
    namespace Ui { class CreateReportDialog; }

    /// \class CreateReportDialog tt3-report/API.hpp
    /// \brief The modal "Create report" dialog.
    class TT3_REPORT_PUBLIC CreateReportDialog final
        :   private QDialog
    {
        Q_OBJECT
        TT3_CANNOT_ASSIGN_OR_COPY_CONSTRUCT(CreateReportDialog)

        //////////
        //  Types
    public:
        /// \brief
        ///     The dialog result after a modal invocation.
        enum class Result
        {
            Ok,     ///< Changes confirme; PublicTask has been created.
            Cancel  ///< The dialog has been cancelled.
        };

        //////////
        //  Construction/destruction
    public:
        /// \brief
        ///     Constructs the dialog.
        /// \param parent
        ///     The parent for the dialog; nullptr == none.
        /// \param reportType
        ///     The report type to initially select, nullptr == auto.
        CreateReportDialog(
                QWidget * parent,
                IReportType * reportType
            );

        /// \brief
        ///     The class destructor.
        virtual ~CreateReportDialog();

        //////////
        //  Operations
    public:
        /// \brief
        ///     Runs the dialog modally.
        /// \return
        ///     The user's choice; on OK a new PublicTask has been created.
        Result          doModal();

        //////////
        //  Implementation
    private:
        //  Helpers
        IReportType *   _selectedReportType() const;
        void            _setSelectedReportType(IReportType * reportType);
        IReportFormat * _selectedReportFormat() const;
        void            _setSelectedReportFormat(IReportFormat * reportFormat);

        //////////
        //  Controls
    private:
        Ui::CreateReportDialog *const   _ui;

        //////////
        //  Signal handlers
    private slots:
        void            _reportTypeComboBoxCurrentIndexChanged(int);
        void            _reportFormatComboBoxCurrentIndexChanged(int);
        void            _browsePushButtonClicked();
        virtual void    accept() override;
        virtual void    reject() override;
    };
}

//  End of tt3-report/CreateReportDialog.hpp
