//
//  tt3-gui/EnterCommentDialog.cpp - tt3::gui::EnterCommentDialog class implementation
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
EnterCommentDialog::EnterCommentDialog(
        ::QWidget * parent,
        tt3::ws::Workspace workspace
    ) : EditStringDialog(
          parent,
          tt3::ws::ObjectTypes::Event::instance()->smallIcon(),
          "Log a comment",
          "Enter comment:",
          "",
          [=](auto c)
          {
              return workspace->validator()->event()->isValidSummary(c);
          })
{
}

//  End of tt3-gui/EnterCommentDialog.cpp
