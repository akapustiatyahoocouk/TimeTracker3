//
//  tt3-gui/ManagePublicActivitiesDialog.hpp - The modal "Manage public activities" dialog
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
    class TT3_GUI_PUBLIC PublicActivityManager;

    namespace Ui { class ManagePublicActivitiesDialog; }

    /// \class ManagePublicActivitiesDialog tt3-gui/API.hpp
    /// \brief The modal "Manage public activities" dialog.
    class TT3_GUI_PUBLIC ManagePublicActivitiesDialog final
        :   private QDialog
    {
        Q_OBJECT
        TT3_CANNOT_ASSIGN_OR_COPY_CONSTRUCT(ManagePublicActivitiesDialog)

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
        ManagePublicActivitiesDialog(
                QWidget * parent,
                tt3::ws::Workspace workspace,
                const tt3::ws::Credentials & credentials
            );

        /// \brief
        ///     The class destructor.
        virtual ~ManagePublicActivitiesDialog();

        //////////
        //  Operations
    public:
        /// \brief
        ///     Runs the dialog modally.
        void        doModal();

        //////////
        //  Controls
    private:
        Ui::ManagePublicActivitiesDialog *const _ui;
        //  Custom controls are created dynamically
        QStackedLayout *        _managerPanelLayout = nullptr;
        PublicActivityManager * _publicActivityManager = nullptr;
    };
}

//  End of tt3-gui/ManagePublicActivitiesDialog.hpp
