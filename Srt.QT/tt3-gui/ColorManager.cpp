//
//  tt3-gui/ColorManager.cpp - tt3::gui::ColorManager class implementation
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

QColor ColorManager::mid(const QColor & a, const QColor & b)
{
    return QColor((a.red() + b.red()) / 2,
                  (a.green() + b.green()) / 2,
                  (a.blue() + b.blue()) / 2,
                  (a.alpha() + b.alpha()) / 2);
}

QColor ColorManager::redder(const QColor & c)
{
    if (c.red() < 128)
    {   //  just add red
        return QColor(c.red() + 128, c.green(), c.blue(), c.alpha());
    }
    else
    {   //  max the red, reduce green and blue
        return QColor(255, qMax(0, c.green() - 128), qMax(0, c.blue()) - 128, c.alpha());
    }
}

QColor ColorManager::greener(const QColor & c)
{
    if (c.green() < 128)
    {   //  just add hreen
        return QColor(c.red(), c.green() + 128, c.blue(), c.alpha());
    }
    else
    {   //  max the green, reduce red and blue
        return QColor(qMax(0, c.red() - 128), 255, qMax(0, c.blue()) - 128, c.alpha());
    }
}

//  End of tt3-gui/ColorManager.cpp
