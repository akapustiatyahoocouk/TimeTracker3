//
//  tt3-gui/DailyWorkQuickReportView.hpp - The "daily work" quick report view.
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
    namespace Ui { class DailyWorkQuickReportView; }

    /// \class DailyWorkQuickReportView tt3-gui/API.hpp
    /// \brief The "daily work" quick report view.
    class TT3_GUI_PUBLIC DailyWorkQuickReportView
        :   public QuickReportView
    {
        Q_OBJECT
        CANNOT_ASSIGN_OR_COPY_CONSTRUCT(DailyWorkQuickReportView)

        //////////
        //  Construction/destruction
    public:
        explicit DailyWorkQuickReportView(
                QWidget * parent
            );

        /// \brief
        ///     The class destructor.
        virtual ~DailyWorkQuickReportView();

        //////////
        //  QuickReportView
    public:
        virtual IQuickReport *  quickReport() const override;

        //////////
        //  Controls
    private:
        Ui::DailyWorkQuickReportView *const _ui;
    };
}

//  End of //  tt3-gui/DailyWorkQuickReportView.hpp

