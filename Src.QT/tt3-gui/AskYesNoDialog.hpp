//
//  tt3-gui/AskYesNoDialog.hpp - the modal "yes/no" dialog
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
    //  The modal "yes/no" dialog
    namespace Ui { class AskYesNoDialog; }

    /// \class AskYesNoDialog "tt3-gui/API.hpp"
    /// \brief
    ///     The modal Yes/No dialog asking the user
    ///     to make a choice.
    class TT3_GUI_PUBLIC AskYesNoDialog
        :   private QDialog
    {
        Q_OBJECT
        CANNOT_ASSIGN_OR_COPY_CONSTRUCT(AskYesNoDialog)

        //////////
        //  Types
    public:
        /// \brief
        ///     The dialog result after a modal invocation
        enum class Result
        {
            Yes,    ///< The user has chosen a Yes option.
            No      ///< The user has chosen a No option.
        };

        //////////
        //  Construction/destruction
    public:
        /// \brief
        ///     Constructs the dialog.
        /// \param parent
        ///     The parent widget for the dialog; nullptr == none.
        /// \param icon
        ///     The dialog icon.
        /// \param title
        ///     The dialog title.
        /// \param prompt
        ///     The dialog prompt; can be long or contain
        ///     multiple '\\n' - separated lines.
        /// \param confirmActionSetting
        ///     The persistent setting backing up the user's choice
        ///     to automaticaly assume a "Yes" answer for all
        ///     subsequent invocations of the dialog; nullptr == none.
        AskYesNoDialog(
                QWidget * parent,
                const QIcon & icon,
                const QString & title,
                const QString & prompt,
                tt3::util::Setting<bool> * confirmActionSetting = nullptr
            );

        /// \brief
        ///     The class destructor.
        virtual ~AskYesNoDialog();

        //////////
        //  Operations
    public:
        /// \brief
        ///     Runs the dialog modally.
        ///\return
        ///     The user's choice.
        Result          doModal();

        //////////
        //  Implementation
    private:
        //  nullptr == question is not backed up by a setting
        tt3::util::Setting<bool> *const _confirmActionSetting;

        //  Helpers
    protected:
        /// \brief For derived classes which need to display error messages
        ::QDialog *     meAsParent() { return this; }

        //////////
        //  Controls
    private:
        Ui::AskYesNoDialog *const   _ui;

        //////////
        //  Signal handlers - "protected" for derived classes to override
    protected slots:
        virtual void    accept() override;
        virtual void    reject() override;
    };

    //  TODO split all dialgs below into dedicated .hpp/.cpp files - one pair for each dialog
    /// \class ConfirmForgetWorkspaceDialog tt3-gui/API.hpp
    /// \brief
    ///     The modal "confirm workspace removal from
    ///     MRU workspaces list" dialog.
    class TT3_GUI_PUBLIC ConfirmForgetWorkspaceDialog final :
        public AskYesNoDialog
    {
        CANNOT_ASSIGN_OR_COPY_CONSTRUCT(ConfirmForgetWorkspaceDialog)

        //////////
        //  Construction/destruction
    public:
        /// \brief
        ///     Constructs the dialog.
        /// \param parent
        ///     The parent widget for the dialog; nullptr == none.
        /// \param workspaceAddress
        ///     The address of the Workspace being removed.
        ConfirmForgetWorkspaceDialog(
                ::QWidget * parent,
                tt3::ws::WorkspaceAddress workspaceAddress
            );
        //  The default destructor is OK
    };

    /// \class ConfirmCloseWorkspaceDialog tt3-gui/API.hpp
    /// \brief The modal "confirm workspace closing" dialog.
    class TT3_GUI_PUBLIC ConfirmCloseWorkspaceDialog final :
        public AskYesNoDialog
    {
        CANNOT_ASSIGN_OR_COPY_CONSTRUCT(ConfirmCloseWorkspaceDialog)

        //////////
        //  Construction/destruction
    public:
        /// \brief
        ///     Constructs the dialog.
        /// \param parent
        ///     The parent widget for the dialog; nullptr == none.
        /// \param workspaceAddress
        ///     The address of the Workspace being closed.
        ConfirmCloseWorkspaceDialog(
                ::QWidget * parent,
                tt3::ws::WorkspaceAddress workspaceAddress
            );
        //  The default destructor is OK
    };

    /// \class ConfirmDropWorkspaceDialog tt3-gui/API.hpp
    /// \brief The modal "confirm closing workspace after relogin" dialog.
    class TT3_GUI_PUBLIC ConfirmDropWorkspaceDialog final :
        public AskYesNoDialog
    {
        CANNOT_ASSIGN_OR_COPY_CONSTRUCT(ConfirmDropWorkspaceDialog)

        //////////
        //  Construction/destruction
    public:
        /// \brief
        ///     Constructs the dialog.
        /// \param parent
        ///     The parent widget for the dialog; nullptr == none.
        /// \param workspaceAddress
        ///     The address of the Workspace being dropped due to re-login.
        ConfirmDropWorkspaceDialog(
                ::QWidget * parent,
                tt3::ws::WorkspaceAddress workspaceAddress
            );
        //  The default destructor is OK
    };

    /// \class ConfirmDestroyWorkspaceDialog tt3-gui/API.hpp
    /// \brief The modal "confirm workspace destruction" dialog
    class TT3_GUI_PUBLIC ConfirmDestroyWorkspaceDialog final :
        public AskYesNoDialog
    {
        CANNOT_ASSIGN_OR_COPY_CONSTRUCT(ConfirmDestroyWorkspaceDialog)

        //////////
        //  Construction/destruction
    public:
        /// \brief
        ///     Constructs the dialog.
        /// \param parent
        ///     The parent widget for the dialog; nullptr == none.
        /// \param workspaceAddress
        ///     The address of the Workspace being destroyed.
        ConfirmDestroyWorkspaceDialog(
                ::QWidget * parent,
                tt3::ws::WorkspaceAddress workspaceAddress
            );
        //  The default destructor is OK
    };

    /// \class ConfirmRestartDialog tt3-gui/API.hpp
    /// \brief The modal "confirm restarting TT3" dialog.
    class TT3_GUI_PUBLIC ConfirmRestartDialog final :
        public AskYesNoDialog
    {
        CANNOT_ASSIGN_OR_COPY_CONSTRUCT(ConfirmRestartDialog)

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
    };

    /// \class ConfirmExitDialog tt3-gui/API.hpp
    /// \brief The modal "confirm exiting TT3" dialog.
    class TT3_GUI_PUBLIC ConfirmExitDialog final :
        public AskYesNoDialog
    {
        CANNOT_ASSIGN_OR_COPY_CONSTRUCT(ConfirmExitDialog)

        //////////
        //  Construction/destruction
    public:
        /// \brief
        ///     Constructs the dialog.
        /// \param parent
        ///     The parent widget for the dialog; nullptr == none.
        explicit ConfirmExitDialog(
                ::QWidget * parent
            );
        //  The default destructor is OK
    };

    /// \class ChooseReloginDialog tt3-gui/API.hpp
    /// \brief The modal "relogin to access workspace" dialog.
    class TT3_GUI_PUBLIC ChooseReloginDialog final :
        public AskYesNoDialog
    {
        CANNOT_ASSIGN_OR_COPY_CONSTRUCT(ChooseReloginDialog)

        //////////
        //  Construction/destruction
    public:
        /// \brief
        ///     Constructs the dialog.
        /// \param parent
        ///     The parent widget for the dialog; nullptr == none.
        /// \param workspaceAddress
        ///     The address of the workspace in question.
        ChooseReloginDialog(
                ::QWidget * parent,
                tt3::ws::WorkspaceAddress workspaceAddress
            );
        //  The default destructor is OK
    };

    /// \class ConfirmResetPageSettingsDialog tt3-gui/API.hpp
    /// \brief The modal "confirm resetting page settings" dialog.
    class TT3_GUI_PUBLIC ConfirmResetPageSettingsDialog final :
        public AskYesNoDialog
    {
        CANNOT_ASSIGN_OR_COPY_CONSTRUCT(ConfirmResetPageSettingsDialog)

        //////////
        //  Construction/destruction
    public:
        /// \brief
        ///     Constructs the dialog.
        /// \param parent
        ///     The parent widget for the dialog; nullptr == none.
        explicit ConfirmResetPageSettingsDialog(
                ::QWidget * parent
            );
        //  The default destructor is OK
    };
}

//  Macro needed for MOC-generated .cpp files
#define TT3_GUI_ASK_YES_NO_DIALOG_DEFINED

//  End of tt3-gui/AskYesNoDialog.hpp
