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
    class TT3_GUI_PUBLIC AskYesNoDialog : private QDialog
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
        AskYesNoDialog(QWidget * parent, const QIcon & icon,
                       const QString & title, const QString & prompt,
                       tt3::util::Setting<bool> * confirmActionSetting = nullptr);
        virtual ~AskYesNoDialog();

        //////////
        //  Operations
    public:
        //  Runs the dialog modally, returning user's choice
        Result          doModal();

        //////////
        //  Implementation
    private:
        //  nullptr == question is not backed up by a setting
        tt3::util::Setting<bool> *const _confirmActionSetting;

        //////////
        //  Controls
    private:
        Ui::AskYesNoDialog *    _ui;

        //////////
        //  Signal handlers
    private slots:
        void            accept() override;
        void            reject() override;
    };

    //  The modal "confirm workspace removal from
    //  MRU workspaces list" dialog
    class TT3_GUI_PUBLIC ConfirmForgetWorkspaceDialog : public AskYesNoDialog
    {
        CANNOT_ASSIGN_OR_COPY_CONSTRUCT(ConfirmForgetWorkspaceDialog)

        //////////
        //  Construction/destruction
    public:
        ConfirmForgetWorkspaceDialog(::QWidget * parent, tt3::ws::WorkspaceAddress workspaceAddress);
    };

    //  The modal "confirm workspace closing" dialog
    class TT3_GUI_PUBLIC ConfirmCloseWorkspaceDialog : public AskYesNoDialog
    {
        CANNOT_ASSIGN_OR_COPY_CONSTRUCT(ConfirmCloseWorkspaceDialog)

        //////////
        //  Construction/destruction
    public:
        ConfirmCloseWorkspaceDialog(::QWidget * parent, tt3::ws::WorkspaceAddress workspaceAddress);
    };

    //  The modal "confirm closing workspace after relogin" dialog
    class TT3_GUI_PUBLIC ConfirmDropWorkspaceDialog : public AskYesNoDialog
    {
        CANNOT_ASSIGN_OR_COPY_CONSTRUCT(ConfirmDropWorkspaceDialog)

        //////////
        //  Construction/destruction
    public:
        ConfirmDropWorkspaceDialog(::QWidget * parent, tt3::ws::WorkspaceAddress workspaceAddress);
    };

    //  The modal "confirm workspace destruction" dialog
    class TT3_GUI_PUBLIC ConfirmDestroyWorkspaceDialog : public AskYesNoDialog
    {
        CANNOT_ASSIGN_OR_COPY_CONSTRUCT(ConfirmDestroyWorkspaceDialog)

        //////////
        //  Construction/destruction
    public:
        ConfirmDestroyWorkspaceDialog(::QWidget * parent, tt3::ws::WorkspaceAddress workspaceAddress);
    };

    //  The modal "confirm restarting TT3" dialog
    class TT3_GUI_PUBLIC ConfirmRestartDialog : public AskYesNoDialog
    {
        CANNOT_ASSIGN_OR_COPY_CONSTRUCT(ConfirmRestartDialog)

        //////////
        //  Construction/destruction
    public:
        explicit ConfirmRestartDialog(::QWidget * parent);
    };

    //  The modal "confirm exiting TT3" dialog
    class TT3_GUI_PUBLIC ConfirmExitDialog : public AskYesNoDialog
    {
        CANNOT_ASSIGN_OR_COPY_CONSTRUCT(ConfirmExitDialog)

        //////////
        //  Construction/destruction
    public:
        explicit ConfirmExitDialog(::QWidget * parent);
    };

    //  The modal "relogin to access workspace" dialog
    class TT3_GUI_PUBLIC ChooseReloginDialog : public AskYesNoDialog
    {
        CANNOT_ASSIGN_OR_COPY_CONSTRUCT(ChooseReloginDialog)

        //////////
        //  Construction/destruction
    public:
        ChooseReloginDialog(::QWidget * parent, tt3::ws::WorkspaceAddress workspaceAddress);
    };

    //  The modal "confirm resetting page settings" dialog
    class TT3_GUI_PUBLIC ConfirmResetPageSettingsDialog : public AskYesNoDialog
    {
        CANNOT_ASSIGN_OR_COPY_CONSTRUCT(ConfirmResetPageSettingsDialog)

        //////////
        //  Construction/destruction
    public:
        explicit ConfirmResetPageSettingsDialog(::QWidget * parent);
    };

    //  The modal "confirm resetting all settings" dialog
    class TT3_GUI_PUBLIC ConfirmResetAllSettingsDialog : public AskYesNoDialog
    {
        CANNOT_ASSIGN_OR_COPY_CONSTRUCT(ConfirmResetAllSettingsDialog)

        //////////
        //  Construction/destruction
    public:
        explicit ConfirmResetAllSettingsDialog(::QWidget * parent);
    };

    //  The modal "restart required" dialog
    class TT3_GUI_PUBLIC RestartRequiredDialog : public AskYesNoDialog
    {
        CANNOT_ASSIGN_OR_COPY_CONSTRUCT(RestartRequiredDialog)

        //////////
        //  Construction/destruction
    public:
        explicit RestartRequiredDialog(::QWidget * parent);
    };

    /// \class ConfirmDestroyUserDialog "tt3-gui/API.hpp"
    /// \brief
    ///     The modal "confirm User destruction" dialog.
    class TT3_GUI_PUBLIC ConfirmDestroyUserDialog : public AskYesNoDialog
    {
        CANNOT_ASSIGN_OR_COPY_CONSTRUCT(ConfirmDestroyUserDialog)

        //////////
        //  Construction/destruction
    public:
        /// @brief
        ///     Constructs the dialog.
        /// @param parent
        ///     The parent widget for the dialog; nullptr == none.
        /// @param user
        ///     The user whose destruction is to be confirmed.
        /// @param credentials
        ///     The credentials to use for accessing User's data.
        /// @exception WorkspaceException
        ///     If an error occurs retrieving User details.
        ConfirmDestroyUserDialog(
                ::QWidget * parent,
                tt3::ws::User user,
                const tt3::ws::Credentials & credentials
            );

        //////////
        //  Implementation
    private:
        //  Helpers
        static auto     _prompt(tt3::ws::User user,
                                const tt3::ws::Credentials & credentials
                            ) -> QString;
    };

    /// \class ConfirmDestroyAccountDialog "tt3-gui/API.hpp"
    /// \brief
    ///     The modal "confirm Account destruction" dialog
    class TT3_GUI_PUBLIC ConfirmDestroyAccountDialog : public AskYesNoDialog
    {
        CANNOT_ASSIGN_OR_COPY_CONSTRUCT(ConfirmDestroyAccountDialog)

        //////////
        //  Construction/destruction
    public:
        /// \brief
        ///     Constructs the dialog.
        /// @param parent
        ///     The parent widget for the dialog; nullptr == none.
        /// @param account
        ///     The account whose destruction is to be confirmed.
        /// @param credentials
        ///     The credentials to use for accessing Account's data.
        /// @exception WorkspaceException
        ///     If an error occurs retrieving Account details.
        ConfirmDestroyAccountDialog(
                ::QWidget * parent,
                tt3::ws::Account account,
                const tt3::ws::Credentials & credentials
            );
    };
}

//  End of tt3-gui/AskYesNoDialog.hpp

