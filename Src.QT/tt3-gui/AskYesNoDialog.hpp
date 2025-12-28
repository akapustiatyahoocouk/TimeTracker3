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
        TT3_CANNOT_ASSIGN_OR_COPY_CONSTRUCT(AskYesNoDialog)

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
        /// \brief   For derived classes which need to display error messages.
        /// \returns "this" as a QDialog*.
        ::QDialog *     meAsParent() { return this; }

        //////////
        //  Controls
    private:
        Ui::AskYesNoDialog *const   _ui;

        //////////
        //  Signal handlers - "protected" for derived classes to override
    protected slots:
        /// \brief Accepts the user's choice (user says Yes).
        virtual void    accept() override;
        /// \brief Rejects the user's choice (user says No).
        virtual void    reject() override;
    };
}

//  Macro needed for MOC-generated .cpp files
#define TT3_GUI_ASK_YES_NO_DIALOG_DEFINED

//  End of tt3-gui/AskYesNoDialog.hpp
