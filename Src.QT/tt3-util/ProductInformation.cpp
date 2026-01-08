//
//  tt3-util/ProductInformation.cpp - tt3::util::ProductInformation class implementation
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
QString ProductInformation::applicationName()
{
    return Component::Resources::instance()->string(RSID(ProductInformation), RID(ApplicationName));
}

QVersionNumber ProductInformation::applicationVersion()
{
    return fromString<QVersionNumber>(TT3_VERSION);
}

QString ProductInformation::applicationStage()
{
    return TT3_STAGE;
}

QString ProductInformation::applicationCopyright()
{
    return Component::Resources::instance()->string(RSID(ProductInformation), RID(ApplicationCopyright), QString(TT3_BUILD_DATE).left(4));
}

QString ProductInformation::applicationBuildNumber()
{
    return TT3_BUILD_DATE "-" TT3_BUILD_TIME;
}

QString ProductInformation::applicationDisplayName()
{
    return Component::Resources::instance()->string(RSID(ProductInformation), RID(ApplicationDisplayName));
}

QString ProductInformation::organizationName()
{
    return Component::Resources::instance()->string(RSID(ProductInformation), RID(OrganizationName));
}

QString ProductInformation::organizationDomain()
{
    return Component::Resources::instance()->string(RSID(ProductInformation), RID(OrganizationDomain));
}

QString ProductInformation::credits()
{
    return Component::Resources::instance()->string(RSID(ProductInformation), RID(Credits));
}

//  End of tt3-util/ProductInformation.cpp
