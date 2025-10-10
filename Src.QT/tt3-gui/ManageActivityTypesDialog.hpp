//
//  tt3-gui/ManageActivityTypesDialog.hpp - The modal "Manage activity types" dialog
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
    class TT3_GUI_PUBLIC ActivityTypeManager;

    namespace Ui { class ManageActivityTypesDialog; }

    /// \class ManageActivityTypesDialog tt3-gui/API.hpp
    /// \brief The modal "Manage activity types" dialog.
    class TT3_GUI_PUBLIC ManageActivityTypesDialog final
        :   private QDialog
    {
        Q_OBJECT
        CANNOT_ASSIGN_OR_COPY_CONSTRUCT(ManageActivityTypesDialog)

        //////////
        //  Construction/destruction
    public:
        /// \brief
        ///     Constructs the dialog.
        /// \param parent
        ///     The parent widget for the dialog, nullptr == none.
        /// \param workspace
        ///     The workspace to manage activity types in.
        /// \param credentials
        ///     The credentials to use for data access.
        ManageActivityTypesDialog(
                QWidget * parent,
                tt3::ws::Workspace workspace,
                const tt3::ws::Credentials & credentials
            );

        /// \brief
        ///     The class destructor.
        virtual ~ManageActivityTypesDialog();

        //////////
        //  Operations
    public:
        /// \brief
        ///     Runs the dialog modally.
        void        doModal();

        //////////
        //  Controls
    private:
        Ui::ManageActivityTypesDialog *const    _ui;
        //  Custom controls are created dynamically
        QStackedLayout *        _managerPanelLayout = nullptr;
        ActivityTypeManager *   _activityTypeManager = nullptr;
    };
}

//  End of tt3-gui/ManageActivityTypesDialog.hpp

