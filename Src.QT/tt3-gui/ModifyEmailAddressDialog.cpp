//
//  tt3-gui/ModifyEmailAddressDialog.cpp - tt3::gui::ModifyEmailAddressDialog class implementation
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
//  Construction/destrution
ModifyEmailAddressDialog::ModifyEmailAddressDialog(
        ::QWidget * parent,
        const QString & emailAddress,
        tt3::ws::Workspace workspace
    ) : EditStringDialog(
            parent,
            QIcon(":/tt3-gui/Resources/Images/Actions/ModifyEmailAddressLarge.png"),
            Component::Resources::instance()->string(
                RSID(ModifyEmailAddressDialog),
                RID(Title)),
            Component::Resources::instance()->string(
                RSID(ModifyEmailAddressDialog),
                RID(Prompt)),
            emailAddress,
            [=](auto a) { return workspace->validator()->principal()->isValidEmailAddress(a); })
{
}

//  End of tt3-gui/ModifyEmailAddressDialog.cpp
