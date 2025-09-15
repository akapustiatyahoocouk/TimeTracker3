//
//  tt3-gui/EditStringDialog.hpp - The modal "Edit string" dialog
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
    namespace Ui { class EditStringDialog; }

    /// \class EditStringDialog tt3-gui/API.hpp
    /// \brief The modal "Edit string" dialog.
    class TT3_GUI_PUBLIC EditStringDialog
        :   private QDialog
    {
        Q_OBJECT
        CANNOT_ASSIGN_OR_COPY_CONSTRUCT(EditStringDialog)

        //////////
        //  Types
    public:
        /// \brief
        ///     The dialog result after a modal invocation
        enum class Result
        {
            Ok,     ///< The user has confirmed the changes.
            Cancel  ///< The user has cancelled the dialog.
        };

        /// \brief
        ///     The validator is a string predicate.
        using Validator = std::function<bool(QString)>;

        //////////
        //  Construction/destruction
    public:
        /// \brief
        ///     Constructs the dialog
        /// \param parent
        ///     The parent widget for the dialog; nullptr == none.
        /// \param icon
        ///     The dialog icon.
        /// \param title
        ///     The dialog title.
        /// \param prompt
        ///     The dialog prompt.
        /// \param initialValue
        ///     The initial edited value.
        /// \param validator
        ///     The value validator, nullptr == none.
        EditStringDialog(
                QWidget * parent,
                const QIcon & icon,
                const QString & title,
                const QString & prompt,
                const QString & initialValue,
                Validator validator = nullptr
            );

        /// \brief
        ///     The class destructor.
        virtual ~EditStringDialog();

        //////////
        //  Operations
    public:
        /// \brief
        ///     Runs the dialog modally.
        /// \return
        ///     The user's choice
        Result          doModal();

        /// \brief
        ///     Returns the string as modified by the user.
        /// \return
        ///     The string as modified by the user.
        QString         editedValue() const { return _editedValue; }

        //////////
        //  Implementation
    private:
        Validator       _validator;
        QString         _editedValue;

        //  Helpers
        void            _refresh();

        //////////
        //  Controls
    private:
        Ui::EditStringDialog *  _ui;

        //////////
        //  Signal handlers
    private slots:
        void            _lineEditTextChanged(QString);
        virtual void    accept() override;
        virtual void    reject() override;
    };

    /// \class AddEmailAddressDialog tt3-gui/API.hpp
    /// \brief The "Add e-mail address" modal dialog.
    class TT3_GUI_PUBLIC AddEmailAddressDialog final : public EditStringDialog
    {
        CANNOT_ASSIGN_OR_COPY_CONSTRUCT(AddEmailAddressDialog)

        //////////
        //  Construction/destruction
    public:
        /// \brief
        ///     Constructs the dialog
        /// \param parent
        ///     The parent widget for the dialog; nullptr == none.
        /// \param validator
        ///     The value validator, nullptr == none.
        AddEmailAddressDialog(
                ::QWidget * parent,
                Validator validator
            );
    };

    /// \class ModifyEmailAddressDialog tt3-gui/API.hpp
    /// \brief The "Modify e-mail address" modal dialog.
    class TT3_GUI_PUBLIC ModifyEmailAddressDialog final : public EditStringDialog
    {
        CANNOT_ASSIGN_OR_COPY_CONSTRUCT(ModifyEmailAddressDialog)

        //////////
        //  Construction/destruction
    public:
        /// \brief
        ///     Constructs the dialog
        /// \param parent
        ///     The parent widget for the dialog; nullptr == none.
        /// \param initialValue
        ///     The initial edited value.
        /// \param validator
        ///     The value validator, cannot be nullptr.
        ModifyEmailAddressDialog(
                ::QWidget * parent,
                const QString & initialValue,
                Validator validator
            );
    };
}

//  End of tt3-gui/EditStringDialog.hpp

