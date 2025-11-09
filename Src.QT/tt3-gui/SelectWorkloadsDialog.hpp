//
//  tt3-gui/SelectWorkloadsDialog.hpp - The modal "Select workloads" dialog
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
    namespace Ui { class SelectWorkloadsDialog; }

    /// \class SelectWorkloadsDialog tt3-gui/API.hpp
    /// \brief The modal "Select workloads" dialog.
    class TT3_GUI_PUBLIC SelectWorkloadsDialog
        :   private QDialog
    {
        Q_OBJECT
        CANNOT_ASSIGN_OR_COPY_CONSTRUCT(SelectWorkloadsDialog)

        //////////
        //  Types
    public:
        /// \brief
        ///     The dialog result after a modal invocation.
        enum class Result
        {
            Ok,     ///< The user has selected a set of workloads.
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
        ///     The workspace to select workloads from.
        /// \param credentials
        ///     The credentials to use for data access.
        /// \param workloads
        ///     The set of workloads to start the selection with.
        /// \exception WorkspaceException
        ///     If an error occurs constructing the dialog.
        SelectWorkloadsDialog(
                QWidget * parent,
                tt3::ws::Workspace workspace,
                const tt3::ws::Credentials & credentials,
                const tt3::ws::Workloads & workloads
            );

        /// \brief
        ///     The class destructor.
        virtual ~SelectWorkloadsDialog();

        //////////
        //  Operations
    public:
        /// \brief
        ///     Runs the dialog modally.
        /// \return
        ///     The user's choice.
        Result      doModal();

        /// \brief
        ///     Returns the set of workloads selected by the user.
        /// \return
        ///     The set of workloads selected by the user.
        auto        selectedWorkloads(
                        ) -> tt3::ws::Workloads;

        //////////
        //  Implementation
    private:
        tt3::ws::Workspace      _workspace;
        tt3::ws::Credentials    _credentials;
        bool        _refreshUnderway = false;

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
        auto        _selectedWorkloads(
                        ) -> tt3::ws::Workloads;
        void        _setSelectedWorkloads(
                            const tt3::ws::Workloads & workloads
                        );
        void        _refresh();
        void        _refreshProjectCheckStates(
                            const tt3::ws::Workloads & selectedWorkloads
                        );
        void        _refreshProjectCheckStates(
                            QTreeWidgetItem * projectItem,
                            const tt3::ws::Workloads & selectedWorkloads
                        );
        void        _refreshWorkStreamCheckStates(
                            const tt3::ws::Workloads & selectedWorkloads
                        );
        void        _refreshWorkStreamCheckStates(
                            QTreeWidgetItem * workStreamItem,
                            const tt3::ws::Workloads & selectedWorkloads
                        );

        //////////
        //  Controls
    private:
        Ui::SelectWorkloadsDialog *const    _ui;
        //  Drawing resources
        TreeWidgetDecorations   _treeWidgetDecorations;
        ListWidgetDecorations   _listWidgetDecorations;

        //////////
        //  Signal handlers
    private slots:
        void            _workloadsTabWidgetCurrentChanged(int);
        void            _projectsFilterLineEditTextChanged(QString);
        void            _workStreamsFilterLineEditTextChanged(QString);
        void            _workloadsListWidgetCurrentRowChanged(int);
        void            _projectsTreeWidgetItemChanged(QTreeWidgetItem * item, int);
        void            _workStreamsTreeWidgetItemChanged(QTreeWidgetItem * item, int);
        void            _showCompletedProjectsCheckBoxStateChanged(int);
        virtual void    accept() override;
        virtual void    reject() override;
    };
}

//  End of tt3-gui/SelectWorkloadsDialog.hpp
