//
//  tt3-gui/ErrorDialog.hpp - The modal "error" dialog
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
    namespace Ui { class ErrorDialog; }

    /// \class ErrorDialog tt3-gui/API.hpp
    /// \brief The modal "error" dialog.
    class TT3_GUI_PUBLIC ErrorDialog
        :   private QDialog
    {
        Q_OBJECT
        CANNOT_ASSIGN_OR_COPY_CONSTRUCT(ErrorDialog)

        //////////
        //  Construction/destruction
    public:
        /// \brief
        ///     Constructs the dialog
        /// \param parent
        ///     The parent widget for the dialog; nullptr == none.
        /// \param errorMessage
        ///     The error message to display.
        ErrorDialog(
                QWidget * parent,
                const QString & errorMessage
            );

        /// \brief
        ///     Constructs the dialog
        /// \param parent
        ///     The parent widget for the dialog; nullptr == none.
        /// \param ex
        ///     The exception whose error message to display.
        ErrorDialog(
                QWidget * parent,
                const tt3::util::Exception & ex
            );

        /// \brief
        ///     Constructs the dialog
        /// \param parent
        ///     The parent widget for the dialog; nullptr == none.
        /// \param ex
        ///     The error whose error message to display.
        ErrorDialog(
                QWidget * parent,
                const tt3::util::Error & ex
            );

        /// \brief
        ///     The class destructor.
        virtual ~ErrorDialog();

        //////////
        //  Operations
    public:
        /// \brief
        ///     Runs the dialog modally.
        void            doModal();

        /// \brief
        ///     Shows the error dialog with the specified error message and no parent.
        /// \param ex
        ///     The exception whose error message to display.
        static void     show(
                                const tt3::util::Exception & ex
                            );

        /// \brief
        ///     Shows the error dialog with the specified error message and no parent.
        /// \param ex
        ///     The error whose error message to display.
        static void     show(
                                const tt3::util::Error & ex
                            );

        /// \brief
        ///     Shows the error dialog with the specified error message and no parent.
        /// \param errorMessage
        ///     The error message to display.
        static void     show(
                                const QString & errorMessage
                            );

        /// \brief
        ///     Shows the error dialog with the specified error message and parent.
        /// \param parent
        ///     The parent widget for the dialog; nullptr == none.
        /// \param ex
        ///     The exception whose error message to display.
        static void     show(
                                QWidget * parent,
                                const tt3::util::Exception & ex
                            );

        /// \brief
        ///     Shows the error dialog with the specified error message and parent.
        /// \param parent
        ///     The parent widget for the dialog; nullptr == none.
        /// \param ex
        ///     The error whose error message to display.
        static void     show(
                                QWidget * parent,
                                const tt3::util::Error & ex
                            );

        /// \brief
        ///     Shows the error dialog with the specified error message and parent.
        /// \param parent
        ///     The parent widget for the dialog; nullptr == none.
        /// \param errorMessage
        ///     The error message to display.
        static void     show(
                                QWidget * parent,
                                const QString & errorMessage
                            );

        //////////
        //  Controls
    private:
        Ui::ErrorDialog *const  _ui;
    };
}

//  End of tt3-gui/ErrorDialog.hpp

