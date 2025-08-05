//
//  tt3/ApplicationComponent.cpp - ApplicationComponent class implementation
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
#include "tt3/API.hpp"

IMPLEMENT_SINGLETON(ApplicationComponent)
ApplicationComponent::ApplicationComponent()
    :   Component("tt3",
                  "TimeTracker3 application",
                  "Defines startup and top-level logic of TimeTracker3 application",
                  "Copyright (C) 2026, Andrey Kapustin",
                  QVersionNumber(1, 0, 0))
{
}

ApplicationComponent::~ApplicationComponent()
{
}

//  End of tt3/ApplicationComponent.cpp
