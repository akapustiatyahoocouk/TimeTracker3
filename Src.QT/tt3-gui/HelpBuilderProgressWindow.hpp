//
//  tt3-gui/HelpBuilderProgressWindow.hpp - The HelpSiteBuilder progress window
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
    namespace Ui { class HelpBuilderProgressWindow; }

    /// \class HelpBuilderProgressWindow tt3-gui/API.hpp
    /// \brief The HelpSiteBuilder progress window.
    class TT3_GUI_PUBLIC HelpBuilderProgressWindow
        :   public QDialog
    {
        Q_OBJECT
        CANNOT_ASSIGN_OR_COPY_CONSTRUCT(HelpBuilderProgressWindow)

        //////////
        //  Construction/destruction
    public:
        HelpBuilderProgressWindow();
        ~HelpBuilderProgressWindow();

        //////////
        //  Operations
    public:
        void            reportProgress(const QString & context, const QString & action);

        //////////
        //  Controls
    private:
        Ui::HelpBuilderProgressWindow *const    _ui;

        //////////
        //  Signal handlers
    private slots:
        void            accept();
        void            reject();
    };
}

//  End of tt3-gui/HelpBuilderProgressWindow.hpp
