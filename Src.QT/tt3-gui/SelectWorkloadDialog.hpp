//
//  tt3-gui/SelectWorkloadDialog.hpp - The modal "Select workload" dialog
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

namespace tt3::gui
{
    namespace Ui { class SelectWorkloadDialog; }

    /// \class SelectWorkloadDialog tt3-gui/API.hpp
    /// \brief The modal "Select workload" dialog.
    class TT3_GUI_PUBLIC SelectWorkloadDialog
        :   private QDialog
    {
        Q_OBJECT
        CANNOT_ASSIGN_OR_COPY_CONSTRUCT(SelectWorkloadDialog)

        //////////
        //  Types
    public:
        /// \brief
        ///     The dialog result after a modal invocation.
        enum class Result
        {
            Ok,     ///< The user has selected a workload.
            Cancel  ///< The user has cancelled the dialog.
        };

        //////////
        //  Construction/destruction
    public:
        /// \brief
        ///     Constructs the dialog.
        /// \param parent
        ///     The parent for the dialog; nullptr == none.
        /// \param workspace
        ///     The workspace to select a workload from.
        /// \param credentials
        ///     The credentials to use for data access.
        /// \param workload
        ///     The workload to start the selection with; nullptr == none.
        /// \exception WorkspaceException
        ///     If an error occurs constructing the dialog.
        SelectWorkloadDialog(
                QWidget * parent,
                tt3::ws::Workspace workspace,
                const tt3::ws::Credentials & credentials,
                tt3::ws::Workload workload
            );

        /// \brief
        ///     The class destructor.
        virtual ~SelectWorkloadDialog();

        //////////
        //  Operations
    public:
        /// \brief
        ///     Runs the dialog modally.
        /// \return
        ///     The user's choice.
        Result      doModal();

        /// \brief
        ///     Returns the workload selected by the user.
        /// \return
        ///     The workload selected by the user; nullptr == none.
        auto        selectedWorkload(
                        ) -> tt3::ws::Workload;

        //////////
        //  Implementation
    private:
        tt3::ws::Workspace      _workspace;
        tt3::ws::Credentials    _credentials;
        bool        _refreshUnderway = false;

        tt3::ws::Workload   _selectedWorkload;

        //  Helpers
        auto        _currentWorkload(
                        ) -> tt3::ws::Workload;
        bool        _setCurrentWorkload(
                            tt3::ws::Workload workload
                        );
        bool        _setCurrentProject(
                            tt3::ws::Project project
                        );
        bool        _setCurrentProject(
                            QTreeWidgetItem * projectItem,
                            tt3::ws::Project project
                        );
        bool        _setCurrentWorkStream(
                            tt3::ws::WorkStream workStream
                        );
        bool        _setCurrentWorkStream(
                            QTreeWidgetItem * workStreamItem,
                            tt3::ws::WorkStream workStream
                        );
        void        _setSelectedWorkload(
                            tt3::ws::Workload workload
                        );
        void        _refresh();
        void        _refreshProjectCheckStates();
        void        _refreshProjectCheckStates(
                            QTreeWidgetItem * projectItem
                        );
        void        _refreshWorkStreamCheckStates();
        void        _refreshWorkStreamCheckStates(
                            QTreeWidgetItem * workStreamItem
                        );
        QStringList _projectDisplayPath(tt3::ws::Project project);
        QString     _prompt(const QString & prompt, tt3::ws::Workload workload);

        //////////
        //  Controls
    private:
        Ui::SelectWorkloadDialog *const _ui;
        //  Drawing resources
        TreeWidgetDecorations   _treeWidgetDecorations;

        //////////
        //  Event handlers
    private slots:
        void            _workloadsTabWidgetCurrentChanged(int);
        void            _projectsFilterLineEditTextChanged(QString);
        void            _workStreamsFilterLineEditTextChanged(QString);
        void            _projectsTreeWidgetItemChanged(QTreeWidgetItem * item, int);
        void            _workStreamsTreeWidgetItemChanged(QTreeWidgetItem * item, int);
        void            _showCompletedProjectsCheckBoxStateChanged(int);
        void            _projectsTreeWidgetItemDoubleClicked(QTreeWidgetItem*,int);
        void            _workStreamsTreeWidgetItemDoubleClicked(QTreeWidgetItem*,int);
        virtual void    accept() override;
        virtual void    reject() override;
    };
}

//  End of tt3-gui/SelectWorkloadDialog.hpp
