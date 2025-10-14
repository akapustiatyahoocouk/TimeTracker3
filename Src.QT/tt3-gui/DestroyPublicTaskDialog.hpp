//
//  tt3-gui/DestroyPublicTaskDialog.hpp - the modal "destroy PublicTask" dialog
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
    /// \class DestroyPublicTaskDialog tt3-gui/API.hpp
    /// \brief The modal "Destroy PublicTask" dialog.
    class TT3_GUI_PUBLIC DestroyPublicTaskDialog final
        :   private AskYesNoDialog
    {
        CANNOT_ASSIGN_OR_COPY_CONSTRUCT(DestroyPublicTaskDialog)

        //////////
        //  Types
    public:
        /// \brief
        ///     The dialog result after a modal invocation.
        enum Result
        {
            Ok,     ///<    Destruction confirmed; the PublicTask has been destroyed.
            Cancel  ///<    The user has cancelled the dialog.
        };

        //////////
        //  Construction/destruction
    public:
        /// @brief
        ///     Constructs the dialog.
        /// @param parent
        ///     The parent widget for the dialog; nullptr == none.
        /// @param publicTask
        ///     The PublicTask whose destruction is to be performed.
        /// @param credentials
        ///     The credentials to use for accessing PublicTask's data.
        /// @exception WorkspaceException
        ///     If an error occurs retrieving PublicTask details.
        DestroyPublicTaskDialog(
            ::QWidget * parent,
            tt3::ws::PublicTask publicTask,
            const tt3::ws::Credentials & credentials
            );
        //  The default destructor is OK

        //////////
        //  Operations
    public:
        /// \brief
        ///     Runs the dialog modally.
        /// \return
        ///     The user's choice; on Ok the PublicTask has been destroyed.
        Result          doModal();

        //////////
        //  Implementation
    private:
        tt3::ws::PublicTask _publicTask;
        tt3::ws::Credentials    _credentials;

        //  Helpers
        static QString  _prompt(tt3::ws::PublicTask publicTask,
                                const tt3::ws::Credentials & credentials);

        //////////
        //  Signal handlers
    private slots:
        virtual void    accept() override;
    };
}

#endif  //  def TT3_GUI_ASK_YES_NO_DIALOG_DEFINED
//  End of tt3-gui/DestroyPublicTaskDialog.hpp
