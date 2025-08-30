//
//  tt3-util/LocaleManager.cpp - tt3::util::LocaleManager class implementation
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
#include "tt3-util/API.hpp"
using namespace tt3::util;

Mutex                LocaleManager::_guard;
QMap<QLocale, QIcon> LocaleManager::_smallIcons;
QMap<QLocale, QIcon> LocaleManager::_largeIcons;

//////////
//  Operations
QIcon LocaleManager::smallIcon(const QLocale & locale)
{
    Lock lock(_guard);

    if (!_smallIcons.contains(locale))
    {
        QFile file(":/tt3-util/Resources/Images/Locales/" + locale.name() + "_Small.png");
        if (file.exists())
        {
            QIcon icon(file.fileName());
            _smallIcons[locale] = icon;
        }
        else
        {
            QIcon icon(":/tt3-util/Resources/Images/Locales/WorldSmall.png");
            _smallIcons[locale] = icon;
        }
    }
    return _smallIcons[locale];
}

QIcon LocaleManager::largeIcon(const QLocale & locale)
{
    Lock lock(_guard);

    if (!_largeIcons.contains(locale))
    {
        QFile file(":/tt3-util/Resources/Images/Locales/" + locale.name() + "_Large.png");
        if (file.exists())
        {
            QIcon icon(file.fileName());
            _largeIcons[locale] = icon;
        }
        else
        {
            QIcon icon(":/tt3-util/Resources/Images/Locales/WorldLarge.png");
            _largeIcons[locale] = icon;
        }
    }
    return _largeIcons[locale];
}

//  End of tt3-util/LocaleManager.cpp
