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
    namespace Ui { class ShowLicenseDialog; }

    /// \class ShowLicenseDialog tt3-gui/API.hpp
    /// \brief The modal "show license" dialog.
    class TT3_GUI_PUBLIC ShowLicenseDialog final
        :   private QDialog
    {
        Q_OBJECT
        TT3_CANNOT_ASSIGN_OR_COPY_CONSTRUCT(ShowLicenseDialog)

        //////////
        //  Construction/destruction
    public:
        /// \brief
        ///     Constructs the dialog.
        /// \param parent
        ///     The parent widget for the dialog; nullptr == none.
        /// \param license
        ///     The license to display.
        ShowLicenseDialog(
                QWidget * parent,
                tt3::util::ILicense * license
            );

        /// \brief
        ///     The class destructor.
        virtual ~ShowLicenseDialog();

        //////////
        //  Operations
    public:
        /// \brief
        ///     Runs the dialog modally.
        void                doModal();

        //////////
        //  Controls
    private:
        Ui::ShowLicenseDialog *const    _ui;
    };
}

//  End of tt3-gui/ShowLicenseDialog.hpp

