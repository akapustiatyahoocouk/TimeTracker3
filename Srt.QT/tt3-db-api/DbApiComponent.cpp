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
using namespace db::api;

IMPLEMENT_SINGLETON(DbApiComponent)
DbApiComponent::DbApiComponent()
    :   Component("tt3-db-api",
                "TimeTracker3 database API",
                "Defines common API for TimeTracker3 databases",
                "Copyright (C) 2026, Andrey Kapustin",
                QVersionNumber(1, 0, 0))
{
}

DbApiComponent::~DbApiComponent()
{
}

//  End of tt3-db-api/DbApiComponent.cpp
