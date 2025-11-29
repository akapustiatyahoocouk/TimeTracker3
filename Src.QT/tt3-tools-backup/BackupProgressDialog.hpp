//
//  tt3-tools-backup/BackupProgressDialog.hpp - The "backup progress" dialog.
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
    namespace Ui { class BackupProgressDialog; }

    /// \class BackupProgressDialog tt3-tools-backup/API.hpp
    /// \brief The "backup progress" dialog.
    class TT3_TOOLS_BACKUP_PUBLIC BackupProgressDialog final
        :   private QDialog
    {
        Q_OBJECT
        CANNOT_ASSIGN_OR_COPY_CONSTRUCT(BackupProgressDialog)

        //////////
        //  Construction
    public:
        /// \brief
        ///     Constructs the dialog.
        /// \param parent
        ///     The parent for the dialog; ullptr == none.
        /// \param backupSource
        ///     The backup source - this is normally the user-readable
        ///     representation of the backed-up workspace address.
        /// \param backupDestination
        ///     The backup destination - this is normally the name of
        ///     the newly created and written backup file.
        BackupProgressDialog(
                QWidget * parent,
                const QString & backupSource,
                const QString & backupDestination
            );

        /// \brief
        ///     The class destructor.
        virtual ~BackupProgressDialog();

        //////////
        //  Operations
    public:
        using QDialog::setVisible;

        /// \brief
        ///     Returns the progress of the backup operation.
        /// \param ratioCompleted
        ///     The ratio of the backup that is completed; 0.0 == just
        ///     started, 1.0 == finished; 0.5 = halfway through.
        void            reportProgress(double ratioCompleted);

        /// \brief
        ///     Checks if the user has requested cancellation of the
        ///     backup process.
        /// \return
        ///     True if the user has requested cancellation of the
        ///     backup process; else false.
        bool            cancelRequested() const { return _cancelRequested; }

        //////////
        //  Implementation
    private:
        std::atomic<bool>   _cancelRequested = false;

        //////////
        //  Controls
    private:
        Ui::BackupProgressDialog *const _ui;

        //////////
        //  Signal handlers
    private slots:
        virtual void    accept() override;
        virtual void    reject() override;
    };
}

//  End of tt3-tools-backup/BackupProgressDialog.hpp
