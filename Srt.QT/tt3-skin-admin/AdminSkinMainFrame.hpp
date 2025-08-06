//
//  tt3-skin-admin/AdminSkinMainFrame.hpp - Main frame of the admin skin
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
#include "tt3-skin-admin/API.hpp"

namespace skin::admin
{
    //  Main frame of the admin skin
    namespace Ui { class AdminSkinMainFrame; }

    class TT3_SKIN_ADMIN_PUBLIC AdminSkinMainFrame : public QMainWindow
    {
        Q_OBJECT
        CANNOT_ASSIGN_OR_COPY_CONSTRUCT(AdminSkinMainFrame)

        //////////
        //  Construction
    public:
        explicit AdminSkinMainFrame(QWidget * parent = nullptr);
        virtual ~AdminSkinMainFrame();

        //////////
        //  Controls
    private:
        Ui::AdminSkinMainFrame *    _ui;
    };
}

//  End of tt3-skin-admin/AdminSkinMainFrame.hpp
