//
//  tt3-gui/ManageQuickPicksListDialog.hpp - The modal "Manage quick picks list" dialog
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
    namespace Ui { class ManageQuickPicksListDialog; }

    /// \class ManageQuickPicksListDialog tt3-gui/API.hpp
    /// \brief The modal "Manage quick picks list" dialog.
    class TT3_GUI_PUBLIC ManageQuickPicksListDialog final
        :   private QDialog
    {
        Q_OBJECT
        CANNOT_ASSIGN_OR_COPY_CONSTRUCT(ManageQuickPicksListDialog)

        //////////
        //  Types
    public:
        /// The dialog result after a modal invocation
        enum class Result
        {
            Ok,     ///< The user has confirmed and saved the changes.
            Cancel  ///< The user has cancelled the dialog.
        };

        //////////
        //  Construction/destruction
    public:
        ManageQuickPicksListDialog(
                QWidget * parent,
                tt3::ws::Account account,
                const tt3::ws::Credentials & credentials
            );
        virtual ~ManageQuickPicksListDialog();

        //////////
        //  Operations
    public:
        /// \brief
        ///     Runs the dialog modally.
        /// @return
        ///     The dialog result; Ok means "changes saved".
        Result          doModal();

        //////////
        //  Implementation
    private:
        tt3::ws::Account        _account;
        tt3::ws::Credentials    _credentials;
        bool                    _trackItemStateChanges = false;

        QList<tt3::ws::Activity>_quickPicksList;

        //  Model
        void            _refreshWorkspaceTree(
                                PrivateActivityManager::_UserModel userModel
                            );
        void            _refreshWorkspaceTree(
                                PrivateTaskManager::_UserModel userModel
                            );

        //  Helpers
        using _ActivityExtractor = std::function<tt3::ws::Activity(QTreeWidgetItem*)>;
        void            _clearAndDisableAllControls();
        void            _refillPublicActivitiesTree();
        void            _refillPublicTasksTree();
        void            _refillPrivateActivitiesTree();
        void            _refillPrivateTasksTree();
        void            _refillQuickPicksListWidget();
        void            _refresh();
        void            _refreshCheckMarks(
                                QTreeWidget * treeWidget,
                                _ActivityExtractor fn
                            );
        void            _refreshCheckMarks(
                                QTreeWidgetItem * item,
                                _ActivityExtractor fn
                            );

        //  Drawing resources
        TreeWidgetDecorations   _decorations;

        //////////
        //  Controls
    private:
        Ui::ManageQuickPicksListDialog *const   _ui;

        //////////
        //  Signal handlers
    private slots:
        void            _publicActivitiesTreeWidgetItemChanged(QTreeWidgetItem * item, int);
        void            _publicTasksTreeWidgetItemChanged(QTreeWidgetItem * item, int);
        void            _privateActivitiesTreeWidgetItemChanged(QTreeWidgetItem * item, int);
        void            _privateTasksTreeWidgetItemChanged(QTreeWidgetItem * item, int);
        void            _quickPicksListWidgetCurrentRowChanged(int);
        void            accept() override;
        void            reject() override;
    };
}

//  End of tt3-gui/ManageQuickPicksListDialog.hpp
