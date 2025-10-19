//
//  tt3-gui/DestroyUserDialog.hpp - the modal "destroy user" dialog
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
    /// \class DestroyUserDialog tt3-gui/API.hpp
    /// \brief The modal "Destroy User" dialog.
    class TT3_GUI_PUBLIC DestroyUserDialog final
        :   private AskYesNoDialog
    {
        CANNOT_ASSIGN_OR_COPY_CONSTRUCT(DestroyUserDialog)

        //////////
        //  Types
    public:
        /// \brief
        ///     The dialog result after a modal invocation.
        enum Result
        {
            Ok,     ///<    Destruction confirmed; the User has been destroyed.
            Cancel  ///<    The user has cancelled the dialog.
        };

        //////////
        //  Construction/destruction
    public:
        /// @brief
        ///     Constructs the dialog.
        /// @param parent
        ///     The parent widget for the dialog; nullptr == none.
        /// @param user
        ///     The User whose destruction is to be performed.
        /// @param credentials
        ///     The credentials to use for accessing User's data.
        /// @exception WorkspaceException
        ///     If an error occurs retrieving User details.
        DestroyUserDialog(
                ::QWidget * parent,
                tt3::ws::User user,
                const tt3::ws::Credentials & credentials
            );
        //  The default destructor is OK

        //////////
        //  Operations
    public:
        /// \brief
        ///     Runs the dialog modally.
        /// \return
        ///     The user's choice; on Ok the User has been destroyed.
        Result          doModal();

        //////////
        //  Implementation
    private:
        tt3::ws::User           _user;
        tt3::ws::Credentials    _credentials;

        //  Helpers
        static QString  _prompt(tt3::ws::User user,
                                const tt3::ws::Credentials & credentials);
        static void     _collectDestructionClosure(
                                tt3::ws::User user,
                                const tt3::ws::Credentials & credentials,
                                qsizetype & worksCount,
                                qsizetype & eventsCount,
                                int64_t & worksDurationMs
                            );
        static void     _collectDestructionClosure(
                                tt3::ws::Account account,
                                const tt3::ws::Credentials & credentials,
                                qsizetype & worksCount,
                                qsizetype & eventsCount,
                                int64_t & worksDurationMs
                            );

        //////////
        //  Signal handlers
    private slots:
        virtual void    accept() override;
    };
}

#endif  //  def TT3_GUI_ASK_YES_NO_DIALOG_DEFINED
//  End of tt3-gui/DestroyUserDialog.hpp
