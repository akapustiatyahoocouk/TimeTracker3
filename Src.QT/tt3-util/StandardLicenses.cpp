//
//  tt3-util/StandardLicenses.cpp - tt3::util::StandardLicenses class implementation
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

//////////
//  Operations
Licenses StandardLicenses::all()
{
    static const Licenses result
    {
        Gpl3::instance()
    };
    return result;
}

    //////////
//  StandardLicenses::Gpl3
TT3_IMPLEMENT_SINGLETON(StandardLicenses::Gpl3)
StandardLicenses::Gpl3::Gpl3() {}
StandardLicenses::Gpl3::~Gpl3() {}

Mnemonic StandardLicenses::Gpl3::mnemonic() const
{
    return M(GPLv3);
}

QString StandardLicenses::Gpl3::displayName() const
{
    static Component::Resources *const resources = Component::Resources::instance();   //  idempotent
    return resources->string(RSID(StandardLicenses), RID(GPLv3.DisplayName));
}

QString StandardLicenses::Gpl3::description() const
{
    static Component::Resources *const resources = Component::Resources::instance();   //  idempotent
    return resources->string(RSID(StandardLicenses), RID(GPLv3.Description));
}

QString StandardLicenses::Gpl3::contentAsHtml() const
{
    Lock _(_contentsGuard);

    QLocale defaultLocale;
    if (_contentsAsHtml.contains(defaultLocale))
    {
        return _contentsAsHtml[defaultLocale];
    }
    //  We need to look, in order, for 3 resources:
    //  1.  :/tt3-util/Resources/Licenses/GPLv3_<ll>_<CC>.html
    //  2.  :/tt3-util/Resources/Licenses/GPLv3_<ll>.html
    //  3.  :/tt3-util/Resources/Licenses/GPLv3_en.html
    //  where <ll> and <CC> are the 2-letter codes of the
    //  language and country of the current default locale
    QString ll = QLocale::languageToCode(defaultLocale.language());
    QString cc = QLocale::territoryToCode(defaultLocale.territory());
    QFile file1(":/tt3-util/Resources/Licenses/GPLv3_" + ll + "_" + cc + ".html");
    QFile file2(":/tt3-util/Resources/Licenses/GPLv3_" + ll + ".html");
    QFile file3(":/tt3-util/Resources/Licenses/GPLv3_en.html");
    QString content;
    if (file1.open(QIODevice::ReadOnly | QIODevice::Text))
    {
        content = QTextStream(&file1).readAll();
    }
    else if (file2.open(QIODevice::ReadOnly | QIODevice::Text))
    {
        content = QTextStream(&file2).readAll();
    }
    else if (file2.open(QIODevice::ReadOnly | QIODevice::Text))
    {
        content = QTextStream(&file3).readAll();
    }
    else
    {
        content = description();
    }
    _contentsAsHtml[defaultLocale] = content;
    return content;
}

//  End of tt3-util/StandardLicenses.cpp
