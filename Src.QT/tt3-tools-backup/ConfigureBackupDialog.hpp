//
//  tt3-tools-backup/ConfigureBackupDialog.hpp - The "configure backup job" dialog.
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
#include "tt3-tools-backup/API.hpp"

namespace tt3::tools::backup
{
    namespace Ui { class ConfigureBackupDialog; }

    /// \class ConfigureBackupDialog tt3-tools-backup/API.hpp
    /// \brief The "configure backup job" dialog.
    class TT3_TOOLS_BACKUP_PUBLIC ConfigureBackupDialog final
        :   private QDialog
    {
        Q_OBJECT
        TT3_CANNOT_ASSIGN_OR_COPY_CONSTRUCT(ConfigureBackupDialog)

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
        explicit ConfigureBackupDialog(QWidget * parent);

        /// \brief
        ///     The class destructor.
        virtual ~ConfigureBackupDialog();

        //////////
        //  Operations
    public:
        /// \brief
        ///     Runs the dialog modally.
        /// \return
        ///     The user's choice.
        Result          doModal();

        /// \brief
        ///     Returns the workspace address selected by the user.
        /// \return
        ///     The workspace address selected by the user or
        ///     nullptr if the user has cancelled the dialog.
        auto            selectedWorkspaceAddress(
                            ) const -> tt3::ws::WorkspaceAddress;

        /// \brief
        ///     Returns the backup destination selected by the user.
        /// \details
        ///     This is the full path of the file where backup shall be written.
        /// \return
        ///     The backup destination selected by the user.
        QString         selectedBackupDestination() const;

        //////////
        //  Implementation
    private:
        tt3::ws::WorkspaceAddress   _customWorkspaceAddress = nullptr;  //  nullptr == not selected
        tt3::ws::WorkspaceAddress   _workspaceAddress = nullptr;  //  nullptr == not selected
        QString         _backupDestination;

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
        Ui::ConfigureBackupDialog *const    _ui;

        //////////
        //  Signal handlers
    private slots:
        void            _workspaceSourceRadioButtonClicked();
        void            _workspaceTypeComboBoxCurrentIndexChanged(int);
        void            _browsePushButtonClicked();
        void            _backupToPushButtonClicked();
        virtual void    accept() override;
        virtual void    reject() override;
    };
}

//  End of tt3-tools-backup/ConfigureBackupDialog.hpp

