//
//  tt3-gui/SelectProjectParentDialog.hpp - The "select new parent for a Project" dialog
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

#ifdef TT3_GUI_PROJECT_MANAGER_DEFINED

namespace tt3::gui
{
    namespace Ui { class SelectProjectParentDialog; }

    /// \class SelectProjectParentDialog tt3-gui/API.hpp
    /// \brief The "select new parent for a Project" dialog.
    class TT3_GUI_PUBLIC SelectProjectParentDialog final
        :   private QDialog
    {
        Q_OBJECT
        CANNOT_ASSIGN_OR_COPY_CONSTRUCT(SelectProjectParentDialog)

        //////////
        //  Types
    public:
        /// \brief
        ///     The dialog result after a modal invocation.
        enum class Result
        {
            Ok,     ///< The user has selected a parent project.
            Cancel  ///< The user has cancelled the dialog.
        };

        //////////
        //  Construction/destruction
    public:
        /// \brief
        ///     Constructs the dialog.
        /// @param parent
        ///     The parent widget for the dialog; nullptr == none.
        /// @param project
        ///     The project to select a new parent for.
        /// @param credentials
        ///     The credentials to use for data access.
        /// @param initialParentProject
        ///     The project to initially select as a parent.
        /// @exception WorkspaceException
        ///     If an error occurs retrieving Project details.
        SelectProjectParentDialog(
                QWidget * parent,
                tt3::ws::Project project,
                const tt3::ws::Credentials & credentials,
                tt3::ws::Project initialParentProject
            );

        /// \brief
        ///     Constructs the dialog.
        /// @param parent
        ///     The parent widget for the dialog; nullptr == none.
        /// @param workspace
        ///     The workspace where a project with the selected
        ///     parent will be created.
        /// @param credentials
        ///     The credentials to use for data access.
        /// @param initialParentProject
        ///     The project to initially select as a parent.
        /// @exception WorkspaceException
        ///     If an error occurs retrieving Project details.
        SelectProjectParentDialog(
                QWidget * parent,
                tt3::ws::Workspace workspace,
                const tt3::ws::Credentials & credentials,
                tt3::ws::Project initialParentProject
            );

        /// \brief
        ///     The class destructor.
        virtual ~SelectProjectParentDialog();

        //////////
        //  Operations
    public:
        /// \brief
        ///     Runs the dialog modally.
        /// @return
        ///     The dialog result; Ok means "parent selected".
        Result              doModal();

        /// \brief
        ///     Returns the new Project parent selected by the user.
        /// \return
        ///     The new Project parent selected by the user.
        auto            selectedParentProject(
                            ) const -> tt3::ws::Project
        {
            return _selectedParentProject;
        }

        //////////
        //  Implementation
    private:
        tt3::ws::Workspace      _workspace;
        tt3::ws::Project        _project;
        tt3::ws::Credentials    _credentials;
        tt3::ws::Project        _selectedParentProject;
        bool                    _trackItemStateChanges = false;

        //  Model
        void            _removeReparentedProject(
                                ProjectManager::_WorkspaceModel workspaceModel
                            );
        void            _removeReparentedProject(
                                ProjectManager::_ProjectModel projectModel
                            );

        //  Helpers
        void            _refresh();
        void            _refreshCheckStates();
        void            _refreshCheckStates(QTreeWidgetItem * item);
        bool            _setSelectedProject(tt3::ws::Project project);
        bool            _setSelectedProject(QTreeWidgetItem * item, tt3::ws::Project project);
        QStringList     _projectDisplayPath(tt3::ws::Project project);
        QString         _prompt(const QString & prompt, tt3::ws::Project project);

        //////////
        //  Controls
    private:
        Ui::SelectProjectParentDialog *const _ui;
        TreeWidgetDecorations   _decorations;

        //////////
        //  Signal handlers
    private slots:
        void            _projectsTreeWidgetItemChanged(QTreeWidgetItem * item, int column);
        void            _projectsTreeWidgetItemDoubleClicked(QTreeWidgetItem * item);
        void            _showCompletedProjectsCheckBoxStateChanged(int);
        virtual void    accept() override;
        virtual void    reject() override;
    };
}

#endif  //  def TT3_GUI_PROJECT_MANAGER_DEFINED
//  End of tt3-gui/SelectProjectParentDialog.hpp
