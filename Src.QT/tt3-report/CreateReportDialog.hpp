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
            Ok,     ///< Report has been configured and generated.
            Cancel  ///< The dialog has been cancelled.
        };

        //////////
        //  Construction/destruction
    public:
        /// \brief
        ///     Constructs the dialog.
        /// \param parent
        ///     The parent for the dialog; nullptr == none.
        /// \param workspace
        ///     The workspace to report from.
        /// \param credentials
        ///     The credentials to use for data access.
        /// \param reportType
        ///     The report type to initially select, nullptr == auto.
        CreateReportDialog(
                QWidget * parent,
                tt3::ws::Workspace workspace,
                const tt3::ws::ReportCredentials & credentials,
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

        /// \brief
        ///     Returns the type of the created report.
        /// \return
        ///     The type of the created report; nullptr if report cancelled.
        IReportType *   reportType() const { return _reportType; }

        /// \brief
        ///     Returns the format of the created report.
        /// \return
        ///     The format of the created report; nullptr if report cancelled.
        IReportFormat * reportFormat() const { return _reportFormat; }

        /// \brief
        ///     Returns the template of the created report.
        /// \return
        ///     The template of the created report; nullptr if report cancelled.
        IReportTemplate*reportTemplate() const { return _reportTemplate; }

        /// \brief
        ///     Returns the file name of the created report.
        /// \return
        ///     The file name of the created report; "" if report cancelled.
        QString         reportDestination() const { return _reportDestination; }

        //////////
        //  Implementation
    private:
        tt3::ws::Workspace _workspace;
        const tt3::ws::ReportCredentials    _credentials;

        IReportType *   _reportType = nullptr;
        IReportFormat * _reportFormat = nullptr;
        IReportTemplate*_reportTemplate = nullptr;
        QString         _reportDestination;

        struct _CancelRequest {};

        //  Helpers
        IReportType *   _selectedReportType() const;
        void            _setSelectedReportType(IReportType * reportType);
        IReportFormat * _selectedReportFormat() const;
        void            _setSelectedReportFormat(IReportFormat * reportFormat);
        IReportTemplate*_selectedReportTemplate() const;
        void            _setSelectedReportTemplate(IReportTemplate * reportTemplate);
        void            _refresh();

        //////////
        //  Controls
    private:
        Ui::CreateReportDialog *const   _ui;
        //  Some controls are created dynamically
        QStackedLayout *    _configuratiokGroupBoxLayout;
        QMap<IReportType*, ReportConfigurationEditor*>  _configurationEditors;  //  no nullptrs!

        //////////
        //  Signal handlers
    private slots:
        void            _reportTypeComboBoxCurrentIndexChanged(int);
        void            _reportFormatComboBoxCurrentIndexChanged(int);
        void            _browsePushButtonClicked();
        void            _editorControlValueChanged();
        virtual void    accept() override;
        virtual void    reject() override;
    };
}

//  End of tt3-report/CreateReportDialog.hpp
