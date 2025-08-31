//
//  tt3-gui/ConfirmCloseWorkspaceDialog.hpp - The modal "Confirm close workspace" dialog
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
    //  The modal "Confirm exit" dialog
    namespace Ui { class ConfirmCloseWorkspaceDialog; }

    class TT3_GUI_PUBLIC ConfirmCloseWorkspaceDialog final : private QDialog
    {
        Q_OBJECT
        CANNOT_ASSIGN_OR_COPY_CONSTRUCT(ConfirmCloseWorkspaceDialog)

        //////////
        //  Types
    public:
        //  The dialog result after a modal invocation
        enum class Result { Yes, No };

        //////////
        //  Construction/destruction
    public:
        explicit ConfirmCloseWorkspaceDialog(QWidget * parent, tt3::ws::Workspace workspace);
        virtual ~ConfirmCloseWorkspaceDialog();

        //////////
        //  Operations
    public:
        //  Runs the dialog modally, returning user's choice
        Result          doModal();

        //////////
        //  Controls
    private:
        Ui::ConfirmCloseWorkspaceDialog *   _ui;

        //////////
        //  Signal handlers
    private slots:
        void            _accept();
        void            _reject();
    };
}

//  End of tt3-gui/ConfirmCloseWorkspaceDialog.hpp

