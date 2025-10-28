//
//  tt3-gui/MessageDialog.hpp - The modal "message" dialog
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
    namespace Ui { class MessageDialog; }

    /// \class MessageDialog tt3-gui/API.hpp
    /// \brief The modal "message" dialog.
    class TT3_GUI_PUBLIC MessageDialog final
        :   private QDialog
    {
        Q_OBJECT
        CANNOT_ASSIGN_OR_COPY_CONSTRUCT(MessageDialog)

        //////////
        //  Construction/destruction
    public:
        /// \brief
        ///     Constructs the dialog
        /// \param parent
        ///     The parent widget for the dialog; nullptr == none.
        /// \param title
        ///     The title to display.
        /// \param message
        ///     The message to display.
        MessageDialog(
                QWidget * parent,
                const QString & title,
                const QString & message
            );

        /// \brief
        ///     The class destructor.
        virtual ~MessageDialog();

        //////////
        //  Operations
    public:
        /// \brief
        ///     Runs the dialog modally.
        void            doModal();

        /// \brief
        ///     Shows the message dialog with the specified message.
        /// \param title
        ///     The dialog title.
        /// \param message
        ///     The message to display.
        static void     show(
                                const QString & title,
                                const QString & message
                            );

        /// \brief
        ///     Shows the message dialog with the specified message and parent.
        /// \param parent
        ///     The parent widget for the dialog; nullptr == none.
        /// \param title
        ///     The dialog title.
        /// \param message
        ///     The message to display.
        static void     show(
                                QWidget * parent,
                                const QString & title,
                                const QString & message
                            );

        //////////
        //  Controls
    private:
        Ui::MessageDialog * _ui;
    };
}

//  End of tt3-gui/MessageDialog.hpp

