//
//  tt3-skin-admin/AdminSkinComponent.cpp - AdminSkinComponent class implementation
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
#include "tt3-skin-admin/API.hpp"
using namespace skin::admin;

//////////
//  Singleton
IMPLEMENT_SINGLETON(AdminSkinComponent)

AdminSkinComponent::AdminSkinComponent() {}
AdminSkinComponent::~AdminSkinComponent() {}

//////////
//  IComponent
QString AdminSkinComponent::mnemonic() const
{
    return "tt3-skin-admin";
}

QString AdminSkinComponent::displayName() const
{
    return "TimeTracker3 Administrator skin";
}

QString AdminSkinComponent::desciption() const
{
    return "Defines GUI Administrator skin for TimeTracker3";
}

QString AdminSkinComponent::copyright() const
{
    return "Copyright (C) 2026, Andrey Kapustin";
}

QVersionNumber AdminSkinComponent::version() const
{
    return QVersionNumber(1, 0, 0);
}

QString AdminSkinComponent::buildNumber() const
{
    return __TIMESTAMP__;
}

//  End of tt3-skin-admin/AdminSkinComponent.cpp
