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
    namespace Ui { class SelectWorkspaceDialog; }

    /// \class SelectWorkspaceDialog tt3-gui/API.hpp
    /// \brief The modal "Open workspace" dialog.
    class TT3_GUI_PUBLIC SelectWorkspaceDialog final
        :   private QDialog
    {
        Q_OBJECT
        CANNOT_ASSIGN_OR_COPY_CONSTRUCT(SelectWorkspaceDialog)

        //////////
        //  Types
    public:
        /// \brief
        ///     Specifies which optional controls to show.
        enum OptionalControls
        {
            None,               ///< Show none of the optional controls.
            OpenModeSelection   ///< Show optional controls for open mode selection.
        };

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
        /// \param parent
        ///     The parent widget for the dialog; nullptr == none.
        /// \param optionalControls
        ///     Specified which optional controls to show.
        SelectWorkspaceDialog(
                QWidget * parent,
                OptionalControls optionalControls
            );

        /// \brief
        ///     The class destructor.
        virtual ~SelectWorkspaceDialog();

        //////////
        //  Operations
    public:
        /// \brief
        ///     Runs the dialog modally.
        /// \return
        ///     The user's choice.
        Result          doModal();

        /// \brief
        ///     Returns the workspace address selected by the user.
        /// \return
        ///     The workspace address selected by the user or
        ///     nullptr if the user has cancelled the dialog.
        tt3::ws::WorkspaceAddress   selectedWorkspaceAddress() { return _workspaceAddress; }

        /// \brief
        ///     Returns the workspace open mode selected by the user.
        /// \return
        ///     The workspace open mode selected by the user or
        ///     OpenMode::Default if open mode selection controls
        ///     were not made visible.
        tt3::ws::OpenMode           selectedOpenMode() { return _openMode; }

        //////////
        //  Implementation
    private:
        tt3::ws::WorkspaceAddress   _workspaceAddress = nullptr;  //  nullptr == not selected
        tt3::ws::OpenMode           _openMode = tt3::ws::OpenMode::Default;

        //  Helpers
        void            _refresh();

        //////////
        //  Controls
    private:
        Ui::SelectWorkspaceDialog *const    _ui;

        //////////
        //  Event handlers
    private slots:
        void            _workspaceTypeComboBoxCurrentIndexChanged(int);
        void            _browsePushButtonClicked();
        void            accept() override;
        void            reject() override;
    };
}

//  End of tt3-gui/SelectWorkspaceDialog.hpp
