//
//  tt3-gui/ConfirmRestartDialog.hpp - the modal "confirm restart" dialog
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
    /// \class ConfirmRestartDialog tt3-gui/API.hpp
    /// \brief The modal "confirm restart" dialog.
    class TT3_GUI_PUBLIC ConfirmRestartDialog final
        :   private AskYesNoDialog
    {
        TT3_CANNOT_ASSIGN_OR_COPY_CONSTRUCT(ConfirmRestartDialog)

        //////////
        //  Construction/destruction
    public:
        /// \brief
        ///     Constructs the dialog.
        /// \param parent
        ///     The parent widget for the dialog; nullptr == none.
        explicit ConfirmRestartDialog(
            ::QWidget * parent
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
//  End of tt3-gui/ConfirmRestartDialog.hpp
