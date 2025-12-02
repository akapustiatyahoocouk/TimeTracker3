//
//  tt3-gui/AddEmailAddressDialog.hpp - The modal "Add e-mail address" dialog
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
    /// \class AddEmailAddressDialog tt3-gui/API.hpp
    /// \brief The "Add e-mail addresss" modal dialog.
    class TT3_GUI_PUBLIC AddEmailAddressDialog final
        :   private EditStringDialog
    {
        TT3_CANNOT_ASSIGN_OR_COPY_CONSTRUCT(AddEmailAddressDialog)

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
        ///     The workspace where the e-mail address will be stored.
        AddEmailAddressDialog(
                ::QWidget * parent,
                tt3::ws::Workspace workspace
            );
        //  The default destructor is OK

        //////////
        //  Operations
    public:
        using EditStringDialog::doModal;

        /// \brief
        ///     Returns the e-mail address as entered by the user.
        /// \return
        ///     The e-mail address as entered by the user.
        QString         emailAddress() const { return EditStringDialog::editedValue(); }
    };
}

#endif//    def TT3_GUI_EDIT_STRING_DIALOG_DEFINED
//  End of tt3-gui/AddEmailAddressDialog.hpp
