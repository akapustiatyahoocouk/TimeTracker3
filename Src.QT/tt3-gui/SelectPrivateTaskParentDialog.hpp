//
//  tt3-gui/SelectPrivateTaskParentDialog.hpp - The "select new parent for a PrivateTask" dialog
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

#ifdef TT3_GUI_PRIVATE_TASK_MANAGER_DEFINED

namespace tt3::gui
{
    namespace Ui { class SelectPrivateTaskParentDialog; }

    /// \class SelectPrivateTaskParentDialog tt3-gui/API.hpp
    /// \brief The "select new parent for a PrivateTask" dialog.
    class TT3_GUI_PUBLIC SelectPrivateTaskParentDialog final
        :   private QDialog
    {
        Q_OBJECT
        TT3_CANNOT_ASSIGN_OR_COPY_CONSTRUCT(SelectPrivateTaskParentDialog)

        //////////
        //  Types
    public:
        /// \brief
        ///     The dialog result after a modal invocation.
        enum class Result
        {
            Ok,     ///< The user has selected a parent task.
            Cancel  ///< The user has cancelled the dialog.
        };

        //////////
        //  Construction/destruction
    public:
        /// \brief
        ///     Constructs the dialog.
        /// @param parent
        ///     The parent widget for the dialog; nullptr == none.
        /// @param privateTask
        ///     The private task to select a new parent for.
        /// @param credentials
        ///     The credentials to use for data access.
        /// @param initialParentTask
        ///     The public task to initially select as a parent.
        /// @exception WorkspaceException
        ///     If an error occurs retrieving PrivateTask details.
        SelectPrivateTaskParentDialog(
                QWidget * parent,
                tt3::ws::PrivateTask privateTask,
                const tt3::ws::Credentials & credentials,
                tt3::ws::PrivateTask initialParentTask
            );

        /// \brief
        ///     Constructs the dialog.
        /// @param parent
        ///     The parent widget for the dialog; nullptr == none.
        /// @param owner
        ///     The user for which a private task with the selected
        ///     parent will be created.
        /// @param credentials
        ///     The credentials to use for data access.
        /// @param initialParentTask
        ///     The public task to initially select as a parent.
        /// @exception WorkspaceException
        ///     If an error occurs retrieving PrivateTask details.
        SelectPrivateTaskParentDialog(
                QWidget * parent,
                tt3::ws::User owner,
                const tt3::ws::Credentials & credentials,
                tt3::ws::PrivateTask initialParentTask
            );

        /// \brief
        ///     The class destructor.
        virtual ~SelectPrivateTaskParentDialog();

        //////////
        //  Operations
    public:
        /// \brief
        ///     Runs the dialog modally.
        /// @return
        ///     The dialog result; Ok means "parent selected".
        Result              doModal();

        /// \brief
        ///     Returns the new PrivateTask parent selected by the user.
        /// \return
        ///     The new PrivateTask parent selected by the user.
        auto            selectedParentTask(
            ) const -> tt3::ws::PrivateTask
        {
            return _selectedParentTask;
        }

        //////////
        //  Implementation
    private:
        tt3::ws::User           _owner;
        tt3::ws::PrivateTask    _privateTask;
        tt3::ws::Credentials    _credentials;
        tt3::ws::PrivateTask    _selectedParentTask;
        bool                    _refreshUnderway = false;

        //  Model
        void            _removeReparentedTask(
                                PrivateTaskManager::_WorkspaceModel workspaceModel
                            );
        void            _removeReparentedTask(
                                PrivateTaskManager::_UserModel userModel
                            );
        void            _removeReparentedTask(
                                PrivateTaskManager::_PrivateTaskModel privateTaskModel
                            );
        void            _refreshWorkspaceTree(
                                PrivateTaskManager::_UserModel userModel
                            );

        //  Helpers
        void            _refresh();
        void            _refreshCheckStates();
        void            _refreshCheckStates(QTreeWidgetItem * item);
        void            _setSelectedUser(tt3::ws::User user);
        bool            _setSelectedPrivateTask(tt3::ws::PrivateTask privateTask);
        bool            _setSelectedPrivateTask(QTreeWidgetItem * item, tt3::ws::PrivateTask privateTask);
        QStringList     _taskDisplayPath(tt3::ws::PrivateTask privateTask);
        QString         _prompt(const QString & prompt, tt3::ws::PrivateTask privateTask);

        //////////
        //  Controls
    private:
        Ui::SelectPrivateTaskParentDialog *const    _ui;
        TreeWidgetDecorations   _decorations;
        QTimer                  _refreshTimer;

        //////////
        //  Signal handlers
    private slots:
        void            _privateTasksTreeWidgetItemChanged(QTreeWidgetItem * item, int column);
        void            _privateTasksTreeWidgetItemDoubleClicked(QTreeWidgetItem * item);
        void            _refreshTimerTimeout();
        virtual void    accept() override;
        virtual void    reject() override;
    };
}

#endif  //  def TT3_GUI_PRIVATE_TASK_MANAGER_DEFINED
//  End of tt3-gui/SelectPrivateTaskParentDialog.hpp
