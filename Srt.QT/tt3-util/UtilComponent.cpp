//
//  tt3-util/UtilComponent.cpp - tt3::util::UtilComponent class implementation
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
//  Singleton
IMPLEMENT_SINGLETON(UtilComponent)
UtilComponent::UtilComponent()
    :   Component("tt3-util",
                "TimeTracker3 utility",
                "Defines utility services for TimeTracker3",
                "Copyright (C) 2026, Andrey Kapustin",
                QVersionNumber(1, 0, 0))
{
}

UtilComponent::~UtilComponent()
{
}

//  End of tt3-util/UtilComponent.cpp
