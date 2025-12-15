//
//  tt3-report/ManageReportTemplatesDialog.hpp - The modal "Manage report templates" dialog
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
    namespace Ui { class ManageReportTemplatesDialog; }

    /// \class ManageReportTemplatesDialog tt3-report/API.hpp
    /// \brief The modal "Manage report templates" dialog.
    class TT3_REPORT_PUBLIC ManageReportTemplatesDialog final
        :   public QDialog
    {
        Q_OBJECT
        TT3_CANNOT_ASSIGN_OR_COPY_CONSTRUCT(ManageReportTemplatesDialog)

        class TT3_REPORT_PUBLIC _PreviewGenerator;

        //////////
        //  Construction/destruction
    public:
        /// \brief
        ///     Constructs the dialog.
        /// \param parent
        ///     The parent for the dialog, nullptr == none.
        explicit ManageReportTemplatesDialog(
                QWidget * parent
            );

        /// \brief
        ///     The class destructor.
        virtual ~ManageReportTemplatesDialog();

        //////////
        //  Operations
    public:
        /// \brief
        ///     Runs the dialog modally.
        void            doModal();

        //////////
        //  Signals - dialog itself always listens
    signals:
        /// \brief
        ///     Raised when the dialog must refresh irself.
        void            refreshRequested();

        /// \brief
        ///     Raised when a preview for a report template becomes available.S
        void            previewAvailable(IReportTemplate*, QString);

        //////////
        //  Implementation
    private:
        QSet<QString>   _previewFileNames;  //  to remove when closing the dialog
        QMap<IReportTemplate*, _PreviewGenerator*>  _previewGenerators;
        QMap<IReportTemplate*, QString> _previews;
        bool            _refreshUnderway = false;

        //  Helpers
        void            _refresh();
        void            _refreshReportTemplateItems(QTreeWidgetItem * parentItem);
        auto            _selectedReportTemplate() -> IReportTemplate *;
        void            _setSelectedReportTemplate(
                                IReportTemplate * reportTemplate
                            );

        //////////
        //  Controls
    private:
        Ui::ManageReportTemplatesDialog *   _ui;
        //  Some tree nodes are statically created
        QTreeWidgetItem *   _predefinedReportsItem;
        QTreeWidgetItem *   _customReportsItem;

        //////////
        //  Signal handlers:
    private slots:
        void            _templateTreeWidgetCurrentItemChanged(QTreeWidgetItem*,QTreeWidgetItem*);
        void            _templateTreeWidgetItemExpanded(QTreeWidgetItem*);
        void            _templateTreeWidgetItemCollapsed(QTreeWidgetItem*);
        void            _exportPushButtonClicked();
        void            _importPushButtonClicked();
        void            _removePushButtonClicked();
        void            _refreshRequested();
        void            _previewAvailable(IReportTemplate * reportTemplate, QString html);

        //////////
        //  Threads
    private:
        class TT3_REPORT_PUBLIC _PreviewGenerator final
            :   public QThread
        {
            TT3_CANNOT_ASSIGN_OR_COPY_CONSTRUCT(_PreviewGenerator)

            //////////
            //  Construction/destruction
        public:
            _PreviewGenerator(
                    ManageReportTemplatesDialog * dialog,
                    IReportTemplate * reportTemplate,
                    const QString & htmlFileName
                );
            virtual ~_PreviewGenerator();

            //////////
            //  QThread
        protected:
            virtual void    run() override;

            //////////
            //  Implementation
        private:
            ManageReportTemplatesDialog *   _dialog;
            IReportTemplate *   _reportTemplate;
            QString         _htmlFileName;
        };
    };
}

//  End of tt3-report/ManageReportTemplatesDialog.hpp

