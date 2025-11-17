//
//  tt3-tools-restore/ConfigureRestoreDialog.hpp - The "configure restore job" dialog.
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
#include "tt3-tools-restore/API.hpp"

namespace tt3::tools::restore
{
    namespace Ui { class ConfigureRestoreDialog; }

    /// \class ConfigureRestoreDialog tt3-tools-restore/API.hpp
    /// \brief The "configure restore job" dialog..
    class TT3_TOOLS_RESTORE_PUBLIC ConfigureRestoreDialog final
        :   private QDialog
    {
        Q_OBJECT
        CANNOT_ASSIGN_OR_COPY_CONSTRUCT(ConfigureRestoreDialog)

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
        explicit ConfigureRestoreDialog(QWidget * parent);

        /// \brief
        ///     The class destructor.
        virtual ~ConfigureRestoreDialog();

        //////////
        //  Operations
    public:
        /// \brief
        ///     Runs the dialog modally.
        /// \return
        ///     The user's choice.
        Result          doModal();

        /// \brief
        ///     Returns the full path to the backup file to restore
        ///     a backed up workspace from.
        /// \return
        ///     The full path to the backup file to restore
        ///     a backed up workspace from.
        QString         restoreSource() const;

        /// \brief
        ///     Returns the workspace address to restore into.
        /// \details
        ///     The workspace will be created anew. If it already
        ///     exists, the restore operation fails.
        /// \return
        ///     The workspace address to restore into.
        auto            workspaceAddress(
            ) const -> tt3::ws::WorkspaceAddress;

        //////////
        //  Implementation
    private:
        QString         _restoreSource;
        tt3::ws::WorkspaceAddress   _workspaceAddress = nullptr;  //  nullptr == not selected

        //  Helpers
        void            _refresh();
        auto            _selectedWorkspaceType(
                            ) -> tt3::ws::WorkspaceType;
        void            _setSelectedWorkspaceType(
                                tt3::ws::WorkspaceType workspaceType
                            );

        //////////
        //  Controls
    private:
        Ui::ConfigureRestoreDialog *const   _ui;

        //////////
        //  Signal handlers
    private slots:
        void            _restoreFromPushButtonClicked();
        void            _workspaceTypeComboBoxCurrentIndexChanged(int);
        void            _browsePushButtonClicked();
        virtual void    accept() override;
        virtual void    reject() override;
    };
}

//  End of tt3-tools-restore/ConfigureRestoreDialog.hpp

