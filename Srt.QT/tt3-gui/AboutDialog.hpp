//
//  tt3-gui/AboutDialog.hpp - The modal "About" dialog
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
    namespace Ui { class AboutDialog; }

    /// \class AboutDialog tt3-gui/API.hpp
    /// \brief The modal "About" dialog.
    class TT3_GUI_PUBLIC AboutDialog final
        :   private QDialog
    {
        Q_OBJECT
        CANNOT_ASSIGN_OR_COPY_CONSTRUCT(AboutDialog)

        //////////
        //  Construction/destruction
    public:
        /// \brief
        ///     Constructs the dialog.
        /// \param parent
        ///     The parent widget for the dialog; nullptr == none.
        explicit AboutDialog(QWidget * parent);

        /// \brief
        ///     The class destructor.
        virtual ~AboutDialog();

        //////////
        //  Operations
    public:
        /// \brief
        ///     Runs the dialog modally
        void            doModal();

        //////////
        //  Implementation
    private:
        QList<tt3::util::ILicense*> _licenses;  //  in display order

        //////////
        //  Controls
    private:
        Ui::AboutDialog *const  _ui;
        QMenu *         _licensesPopupMenu = nullptr;

        //////////
        //  Signal handlers
    private slots:
        void            _showLicensePushButtonClicked();
        void            _showConfigurationPushButtonClicked();
        void            _onShowLicense();
    };
}

//  End of tt3-gui/AboutDialog.hpp
