//
//  tt3-tools-restore/RestoreProgressDialog.hpp - The "restore progress" popup window.
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
    namespace Ui { class RestoreProgressDialog; }

    /// \class RestoreProgressDialog tt3-tools-restore/API.hpp
    /// \brief The "restore progress" popup window.
    class TT3_TOOLS_RESTORE_PUBLIC RestoreProgressDialog final
        :   private QDialog
    {
        Q_OBJECT
        TT3_CANNOT_ASSIGN_OR_COPY_CONSTRUCT(RestoreProgressDialog)

        //////////
        //  Construction
    public:
        /// \brief
        ///     Constructs the dialog.
        /// \param parent
        ///     The parent for the dialog; nullptr == none.
        /// \param restoreDestination
        ///     The restore destination - normally the user-readable
        ///     form of the address of the workspace into which data is restored.
        /// \param restoreSource
        ///     The restore source - normally the name of the backup file
        ///     written earlier from which data is restored.
        RestoreProgressDialog(
                QWidget * parent,
                const QString & restoreDestination,
                const QString & restoreSource
            );
        virtual ~RestoreProgressDialog();

        //////////
        //  Operations
    public:
        using QDialog::setVisible;

        /// \brief
        ///     Reports the progress of the restore operationn.
        /// \param ratioCompleted
        ///     The ration of the restore work completed; 0.0 ==
        ///     just started, 1.0 == finished, 0.5 == halfway through.
        void            reportProgress(double ratioCompleted);

        /// \brief
        ///     Checks if the user has requested cancellation
        ///     of the restore process.
        /// \return
        ///     True if the user has requested cancellation of
        ///     the restore process, else false.
        bool            cancelRequested() const { return _cancelRequested; }

        //////////
        //  Implementation
    private:
        std::atomic<bool>   _cancelRequested = false;


        //////////
        //  Controls
    private:
        Ui::RestoreProgressDialog *const    _ui;

        //////////
        //  Signal handlers
    private slots:
        virtual void    accept() override;
        virtual void    reject() override;
    };
}

//  End of tt3-tools-restore/RestoreProgressDialog.hpp
