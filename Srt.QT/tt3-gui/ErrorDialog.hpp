//
//  tt3-gui/ErrorDialog.hpp - the modal "error" dialog
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
    //  The modal "error" dialog
    class TT3_GUI_PUBLIC ErrorDialog final
    {
        UTILITY_CLASS(ErrorDialog)

        //////////
        //  Operations
    public:
        //  TODO document
        static void     show(const tt3::util::Exception & ex);
        static void     show(const std::exception & ex);
        static void     show(const QString & errorMessage);
        static void     show();
        static void     show(QWidget * parent, const tt3::util::Exception & ex);
        static void     show(QWidget * parent, const std::exception & ex);
        static void     show(QWidget * parent, const QString & errorMessage);
        static void     show(QWidget * parent);
    };
}

//  End of tt3-gui/ErrorDialog.hpp
