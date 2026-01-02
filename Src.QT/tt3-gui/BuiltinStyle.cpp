//
//  tt3-gui/BuiltinStyle.cpp - tt3::gui::BuiltinStyle class implementation
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
//  Construction/destroucion - internal only
BuiltinStyle::BuiltinStyle(const QString & styleName)
    :   _styleName(styleName.toLower())
{
}

//////////
//  IStyle
auto BuiltinStyle::mnemonic() const -> Mnemonic
{
    return tt3::util::Mnemonic(_styleName);
}

QString BuiltinStyle::displayName() const
{
    if (_displayName.isEmpty())
    {   //  Evaluate ONCE
        if (auto style = QStyleFactory::create(_styleName))
        {
            _displayName = style->name();
            delete style;
        }
        else
        {   //  Fall back to default
            _displayName = _styleName;
        }
    }
    return _displayName;
}

QString BuiltinStyle::description() const
{
    return displayName();
}

QIcon BuiltinStyle::smallIcon() const
{
    static const QIcon icon(":/tt3-gui/Resources/Images/Misc/StyleSmall.png");
    return icon;
}

QIcon BuiltinStyle::largeIcon() const
{
    static const QIcon icon(":/tt3-gui/Resources/Images/Misc/StyleLarge.png");
    return icon;
}

void BuiltinStyle::apply()
{
    QApplication::setStyle(_styleName);
}

//  End of tt3-gui/BuiltinStyle.cpp
