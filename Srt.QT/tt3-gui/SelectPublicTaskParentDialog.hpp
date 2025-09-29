//
//  tt3-gui/SelectPublicTaskParentDialog.hpp - The "select new parent for a PublicTask" dialog
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
    namespace Ui { class SelectPublicTaskParentDialog; }

    /// \class SelectPublicTaskParentDialog tt3-gui/API.hpp
    /// \brief The "select new parent for a PublicTask" dialog.
    class TT3_GUI_PUBLIC SelectPublicTaskParentDialog final
        :   private QDialog
    {
        Q_OBJECT
        CANNOT_ASSIGN_OR_COPY_CONSTRUCT(SelectPublicTaskParentDialog)

        //////////
        //  Types
    public:
        /// \brief
        ///     The dialog result after a modal invocation.
        enum class Result
        {
            Ok,     ///< The user has selected a workspace.
            Cancel  ///< The user has cancelled the dialog.
        };

        //////////
        //  Construction/destruction
    public:
        /// \brief
        ///     Constructs the dialog.
        /// @param parent
        ///     The parent widget for the dialog; nullptr == none.
        /// @param publicTask
        ///     The public task to select a new parent for.
        /// @param credentials
        ///     The credentials to use for data access.
        /// @exception WorkspaceException
        ///     If an error occurs retrieving PublicTask details.
        SelectPublicTaskParentDialog(
                QWidget * parent,
                tt3::ws::PublicTask publicTask,
                const tt3::ws::Credentials & credentials
            );
        virtual ~SelectPublicTaskParentDialog();

        //////////
        //  Operations
    public:
        /// \brief
        ///     Runs the dialog modally.
        /// @return
        ///     The dialog result; Ok means "parent selected".
        Result              doModal();

        /// \brief
        ///     Returns the new PublicTask parent selected by the user.
        /// \return
        ///     The new PublicTask parent selected by the user.
        auto            selectedParentTask(
            ) const -> tt3::ws::PublicTask
        {
            return _selectedParentTask;
        }

        //////////
        //  Implementation
    private:
        tt3::ws::PublicTask     _publicTask;
        tt3::ws::Credentials    _credentials;
        tt3::ws::PublicTask     _selectedParentTask;

        //  Helpers
        void            _refresh();

        //////////
        //  Controls
    private:
        Ui::SelectPublicTaskParentDialog *const _ui;

        //////////
        //  Signal handlers
    private slots:
        virtual void    accept() override;
        virtual void    reject() override;
    };
}

//  End of tt3-gui/SelectPublicTaskParentDialog.hpp

