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
        BackupProgressDialog(
                QWidget * parent,
                const QString & backupSource,
                const QString & backupDestination
            );
        virtual ~BackupProgressDialog();

        //////////
        //  Operations
    public:
        using QDialog::setVisible;

        void        reportProgress(double ratioCompleted);

        //////////
        //  Controls
    private:
        Ui::BackupProgressDialog *const _ui;

        //////////
        //  Signal handlers
    private slots:
        void            accept();
        void            reject();
    };
}

//  End of tt3-tools-backup/BackupProgressDialog.hpp
