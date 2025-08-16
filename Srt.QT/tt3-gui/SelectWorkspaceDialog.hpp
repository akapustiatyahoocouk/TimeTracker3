//
//  tt3-gui/SelectWorkspaceDialog.hpp - The modal "Open workspace" dialog
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
    //  The modal "Open workspace" dialog
    namespace Ui { class SelectWorkspaceDialog; }

    class TT3_GUI_PUBLIC SelectWorkspaceDialog : public QDialog
    {
        Q_OBJECT
        CANNOT_ASSIGN_OR_COPY_CONSTRUCT(SelectWorkspaceDialog)

        //////////
        //  Construction/destruction
    public:
        explicit SelectWorkspaceDialog(QWidget * parent = nullptr);
        virtual ~SelectWorkspaceDialog();

        //////////
        //  Operations
    public:
        //  The workspace address selected by the user or an
        //  invalid workspace address if the user has cancelled
        //  the dialog.
        tt3::ws::WorkspaceAddress   selectedWorkspaceAddress() { return _workspaceAddress; }

        //////////
        //  Implementation
    private:
        tt3::ws::WorkspaceAddress   _workspaceAddress;  //  invalid == not selected

        //  Helpers
        void            _refresh();

        //////////
        //  Controls
    private:
        Ui::SelectWorkspaceDialog *   _ui;

        //////////
        //  Event handlers
    private slots:
        void            _workspaceTypeComboBoxCurrentIndexChanged(int);
        void            _browsePushButtonClicked();
    };
}

//  End of tt3-gui/SelectWorkspaceDialog.hpp
