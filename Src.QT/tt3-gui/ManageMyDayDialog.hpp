//
//  tt3-gui/ManageMyDayDialog.hpp - The modal "Manage my day" dialog
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
    class TT3_GUI_PUBLIC MyDayManager;

    namespace Ui { class ManageMyDayDialog; }

    /// \class ManageMyDayDialog tt3-gui/API.hpp
    /// \brief The modal "Manage my day" dialog.
    class TT3_GUI_PUBLIC ManageMyDayDialog final
        :   private QDialog
    {
        Q_OBJECT
        CANNOT_ASSIGN_OR_COPY_CONSTRUCT(ManageMyDayDialog)

        //////////
        //  Construction/destruction
    public:
        /// \brief
        ///     Constructs the dialog.
        /// \param parent
        ///     The parent widget for the dialog, nullptr == none.
        /// \param workspace
        ///     The workspace to manage public activities in.
        /// \param credentials
        ///     The credentials to use for data access.
        ManageMyDayDialog(
                QWidget * parent,
                tt3::ws::Workspace workspace,
                const tt3::ws::Credentials & credentials
            );

        /// \brief
        ///     The class destructor.
        virtual ~ManageMyDayDialog();

        //////////
        //  Operations
    public:
        /// \brief
        ///     Runs the dialog modally.
        void        doModal();

        //////////
        //  Controls
    private:
        Ui::ManageMyDayDialog *const _ui;
        //  Custom controls are created dynamically
        QStackedLayout *    _managerPanelLayout = nullptr;
        MyDayManager *      _myDayManager = nullptr;
    };
}

//  End of tt3-gui/ManageMyDayDialog.hpp
