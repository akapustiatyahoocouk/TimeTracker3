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

struct LocaleManager::_Impl
{
    Mutex                guard;
    QMap<QLocale, QIcon> smallIcons;
    QMap<QLocale, QIcon> largeIcons;
};

//////////
//  Operations
QIcon LocaleManager::smallIcon(const QLocale & locale)
{
    _Impl * impl = _impl();
    Lock lock(impl->guard);

    if (!impl->smallIcons.contains(locale))
    {
        QFile file(":/tt3-util/Resources/Images/Locales/" + locale.name() + "_Small.png");
        if (file.exists())
        {
            QIcon icon(file.fileName());
            impl->smallIcons[locale] = icon;
        }
        else
        {
            QIcon icon(":/tt3-util/Resources/Images/Locales/WorldSmall.png");
            impl->smallIcons[locale] = icon;
        }
    }
    return impl->smallIcons[locale];
}

QIcon LocaleManager::largeIcon(const QLocale & locale)
{
    _Impl * impl = _impl();
    Lock lock(impl->guard);

    if (!impl->largeIcons.contains(locale))
    {
        QFile file(":/tt3-util/Resources/Images/Locales/" + locale.name() + "_Large.png");
        if (file.exists())
        {
            QIcon icon(file.fileName());
            impl->largeIcons[locale] = icon;
        }
        else
        {
            QIcon icon(":/tt3-util/Resources/Images/Locales/WorldLarge.png");
            impl->largeIcons[locale] = icon;
        }
    }
    return impl->largeIcons[locale];
}

QString LocaleManager::displayName(const QLocale & locale)
{
    return QLocale::languageToString(locale.language()) +
           " (" +
           QLocale::territoryToString(locale.territory()) +
           ")";
}

//////////
//  Implementation helpers
LocaleManager::_Impl * LocaleManager::_impl()
{
    static _Impl impl;
    return &impl;
}

//  End of tt3-util/LocaleManager.cpp
