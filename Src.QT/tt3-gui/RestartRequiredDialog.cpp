//
//  tt3-gui/RestartRequiredDialog.cpp - tt3::gui::RestartRequiredDialog class implementation
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
#include "tt3-gui/API.hpp"
using namespace tt3::gui;

//////////
//  Construction/destruction
RestartRequiredDialog::RestartRequiredDialog(
        ::QWidget * parent
    ) : AskYesNoDialog(
            parent,
            QIcon(":/tt3-gui/Resources/Images/Actions/RestartLarge.png"),
            Component::Resources::instance()->string(
                RSID(RestartRequiredDialog),
                RID(Title)),
            Component::Resources::instance()->string(
                RSID(RestartRequiredDialog),
                RID(Prompt)))
{
}

//  End of tt3-gui/RestartRequiredDialog.cpp
