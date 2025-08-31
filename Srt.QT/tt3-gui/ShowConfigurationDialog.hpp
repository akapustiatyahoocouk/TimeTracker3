//
//  s - The modal "show configuration" dialog
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
    //  The modal "show configuration" dialog
    namespace Ui { class ShowConfigurationDialog; }

    class TT3_GUI_PUBLIC ShowConfigurationDialog final : private QDialog
    {
        Q_OBJECT
        CANNOT_ASSIGN_OR_COPY_CONSTRUCT(ShowConfigurationDialog)

        //////////
        //  Construction/destruction
    public:
        explicit ShowConfigurationDialog(QWidget * parent);
        virtual ~ShowConfigurationDialog();

        //////////
        //  Operations
    public:
        //  Runs the dialog modally
        void            doModal();

        //////////
        //  Implementation
    private:
        //  Helpers
        void            _refresh();

        //////////
        //  Controls
    private:
        Ui::ShowConfigurationDialog *   _ui;

        //////////
        //  Signal handlers
    private slots:
        void            _configurationTreeWidgetCurrentItemChanged(QTreeWidgetItem*,QTreeWidgetItem*);
        void            _showLicensePushButtonClicked();
    };
}

//  End of tt3-gui/ShowConfigurationDialog.hpp

