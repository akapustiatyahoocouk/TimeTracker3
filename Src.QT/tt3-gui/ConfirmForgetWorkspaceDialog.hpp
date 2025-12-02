//
//  tt3-gui/ConfirmForgetWorkspaceDialog.hpp - the modal "confirm workspace removal from MRU list" dialog
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

#ifdef TT3_GUI_ASK_YES_NO_DIALOG_DEFINED

    namespace tt3::gui
{
    /// \class ConfirmForgetWorkspaceDialog tt3-gui/API.hpp
    /// \brief
    ///     The modal "confirm workspace removal from
    ///     MRU workspaces list" dialog.
    class TT3_GUI_PUBLIC ConfirmForgetWorkspaceDialog final
        :   private AskYesNoDialog
    {
        TT3_CANNOT_ASSIGN_OR_COPY_CONSTRUCT(ConfirmForgetWorkspaceDialog)

        //////////
        //  Construction/destruction
    public:
        /// \brief
        ///     Constructs the dialog.
        /// \param parent
        ///     The parent widget for the dialog; nullptr == none.
        /// \param workspaceAddress
        ///     The address of the workspace to donfirm destruction of.
        ConfirmForgetWorkspaceDialog(
            ::QWidget * parent,
            tt3::ws::WorkspaceAddress workspaceAddress
            );
        //  The default destructor is OK

        //////////
        //  Operations
    public:
        using Result = AskYesNoDialog::Result;
        using AskYesNoDialog::doModal;
    };
}

#endif  //  def TT3_GUI_ASK_YES_NO_DIALOG_DEFINED
//  End of tt3-gui/ConfirmForgetWorkspaceDialog.hpp
