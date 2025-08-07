//
//  tt3-db-api/DbApiComponent.cpp - AdminSkinComponent class implementation
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
#include "tt3-db-api/API.hpp"
using namespace tt3::db::api;

//////////
//  Singleton
IMPLEMENT_SINGLETON(DbApiComponent)

DbApiComponent::DbApiComponent() {}
DbApiComponent::~DbApiComponent() {}

//////////
//  IComponent
QString DbApiComponent::mnemonic() const
{
    return "tt3-db-api";
}

QString DbApiComponent::displayName() const
{
    return "TimeTracker3 database API";
}

QString DbApiComponent::desciption() const
{
    return "Defines common API for TimeTracker3 databases";
}

QString DbApiComponent::copyright() const
{
    return "Copyright (C) 2026, Andrey Kapustin";
}

QVersionNumber DbApiComponent::version() const
{
    return QVersionNumber(1, 0, 0);
}

QString DbApiComponent::buildNumber() const
{
    return __TIMESTAMP__;
}

//  End of tt3-db-api/DbApiComponent.cpp
