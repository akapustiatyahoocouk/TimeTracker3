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
    /// \brief The "restore progress" popup window..
    class TT3_TOOLS_RESTORE_PUBLIC RestoreProgressDialog final
        :   private QDialog
    {
        Q_OBJECT
        CANNOT_ASSIGN_OR_COPY_CONSTRUCT(RestoreProgressDialog)

        //////////
        //  Construction
    public:
        RestoreProgressDialog(
                QWidget * parent,
                const QString & restoreSource,
                const QString & restoreDestination
            );
        virtual ~RestoreProgressDialog();

        //////////
        //  Operations
    public:
        using QDialog::setVisible;

        void            reportProgress(double ratioCompleted);
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
