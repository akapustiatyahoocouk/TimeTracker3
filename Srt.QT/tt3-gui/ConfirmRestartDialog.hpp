//
//  tt3-gui/ConfirmRestartDialog.hpp - The modal "Confirm restart" dialog
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
    namespace Ui { class ConfirmRestartDialog; }

    class TT3_GUI_PUBLIC ConfirmRestartDialog final : private QDialog
    {
        Q_OBJECT
        CANNOT_ASSIGN_OR_COPY_CONSTRUCT(ConfirmRestartDialog)

        //////////
        //  Types
    public:
        //  The dialog result after a modal invocation
        enum class Result { Yes, No };

        //////////
        //  Construction/destruction
    public:
        explicit ConfirmRestartDialog(QWidget * parent);
        virtual ~ConfirmRestartDialog();

        //////////
        //  Operations
    public:
        //  Runs the dialog modally, returning user's choice
        Result          doModal();

        //////////
        //  Controls
    private:
        Ui::ConfirmRestartDialog *  _ui;

        //////////
        //  Signal handlers
    private slots:
        void            accept() override;
        void            reject() override;
    };
}

//  End of tt3-gui/ConfirmRestartDialog.hpp

