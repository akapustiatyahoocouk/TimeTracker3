//
//  tt3-gui/EnterCommentDialog.hpp - The modal "Enter comment" dialog
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

#ifdef TT3_GUI_EDIT_STRING_DIALOG_DEFINED

    namespace tt3::gui
{
    namespace Ui { class EditStringDialog; }

    /// \class EnterCommentDialog tt3-gui/API.hpp
    /// \brief The "Enter comment" modal dialog.
    class TT3_GUI_PUBLIC EnterCommentDialog final
        :   private EditStringDialog
    {
        TT3_CANNOT_ASSIGN_OR_COPY_CONSTRUCT(EnterCommentDialog)

        //////////
        //  Types
    public:
        /// \brief
        ///     The dialog result after a modal invocation.
        using Result = EditStringDialog::Result;

        //////////
        //  Construction/destruction
    public:
        /// \brief
        ///     Constructs the dialog
        /// \param parent
        ///     The parent widget for the dialog; nullptr == none.
        /// \param workspace
        ///     The Workspace where the comment will be stored as an Event.
        EnterCommentDialog(
                ::QWidget * parent,
                tt3::ws::Workspace workspace
            );
        //  The default destructor is OK

        //////////
        //  Operations
    public:
        using EditStringDialog::doModal;

        /// \brief
        ///     Returns the comment as entered by the user.
        /// \return
        ///     The comment as entered by the user.
        QString         comment() const { return EditStringDialog::editedValue(); }
    };
}

#endif//    def TT3_GUI_EDIT_STRING_DIALOG_DEFINED
//  End of tt3-gui/EnterTaskCompletionCommentDialog.hpp
