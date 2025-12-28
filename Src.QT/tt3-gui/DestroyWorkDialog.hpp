//
//  tt3-gui/DestroyWorkDialog.hpp - the modal "destroy Work" dialog
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
    /// \class DestroyWorkDialog tt3-gui/API.hpp
    /// \brief The modal "Destroy Work" dialog.
    class TT3_GUI_PUBLIC DestroyWorkDialog final
        :   private AskYesNoDialog
    {
        TT3_CANNOT_ASSIGN_OR_COPY_CONSTRUCT(DestroyWorkDialog)

        //////////
        //  Types
    public:
        /// \brief
        ///     The dialog result after a modal invocation.
        enum Result
        {
            Ok,     ///<    Destruction confirmed; the Work has been destroyed.
            Cancel  ///<    The user has cancelled the dialog.
        };

        //////////
        //  Construction/destruction
    public:
        /// @brief
        ///     Constructs the dialog.
        /// @param parent
        ///     The parent widget for the dialog; nullptr == none.
        /// @param work
        ///     The Work whose destruction is to be performed.
        /// @param credentials
        ///     The credentials to use for accessing Work's data.
        /// @exception WorkspaceException
        ///     If an error occurs retrieving Work details.
        DestroyWorkDialog(
                ::QWidget * parent,
                tt3::ws::Work work,
                const tt3::ws::Credentials & credentials
            );
        //  The default destructor is OK

        //////////
        //  Operations
    public:
        /// \brief
        ///     Runs the dialog modally.
        /// \return
        ///     The user's choice; on Ok the PublicActivity has been destroyed.
        Result          doModal();

        //////////
        //  Implementation
    private:
        tt3::ws::Work           _work;
        tt3::ws::Credentials    _credentials;

        //  Helpers
        static QString  _prompt(tt3::ws::Work work,
                                const tt3::ws::Credentials & credentials);

        //////////
        //  Signal handlers
    private slots:
        virtual void    accept() override;
    };
}

#endif  //  def TT3_GUI_ASK_YES_NO_DIALOG_DEFINED
//  End of tt3-gui/DestroyWorkDialog.hpp
