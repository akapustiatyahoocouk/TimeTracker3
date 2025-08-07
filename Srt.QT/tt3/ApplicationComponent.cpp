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
using namespace tt3;

IMPLEMENT_SINGLETON(ApplicationComponent)
ApplicationComponent::ApplicationComponent() {}
ApplicationComponent::~ApplicationComponent() {}

//////////
//  IComponent
QString ApplicationComponent::mnemonic() const
{
    return "tt3";
}

QString ApplicationComponent::displayName() const
{
    return "TimeTracker3 application";
}

QString ApplicationComponent::desciption() const
{
    return "Defines startup and top-level logic of TimeTracker3 application";
}

QString ApplicationComponent::copyright() const
{
    return "Copyright (C) 2026, Andrey Kapustin";
}

QVersionNumber ApplicationComponent::version() const
{
    return QVersionNumber(1, 0, 0);
}

QString ApplicationComponent::buildNumber() const
{
    return __TIMESTAMP__;
}

//  End of tt3/ApplicationComponent.cpp
