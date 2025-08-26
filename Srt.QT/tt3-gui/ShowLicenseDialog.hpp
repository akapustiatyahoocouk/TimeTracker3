//
//  tt3-gui/ShowLicenseDialog.hpp - The modal "show license" dialog
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
    //  The modal "login" dialog
    namespace Ui { class ShowLicenseDialog; }

    class TT3_GUI_PUBLIC ShowLicenseDialog final : public QDialog
    {
        Q_OBJECT
        CANNOT_ASSIGN_OR_COPY_CONSTRUCT(ShowLicenseDialog)

        //////////
        //  Construction/destruction
    public:
        ShowLicenseDialog(QWidget * parent, tt3::util::ILicense * license);
        virtual ~ShowLicenseDialog();

        //////////
        //  Controls
    private:
        Ui::ShowLicenseDialog * _ui;
    };
}

//  End of tt3-gui/ShowLicenseDialog.hpp

