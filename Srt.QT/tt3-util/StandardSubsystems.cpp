//
//  tt3-util/StandardSubsystems.cpp - tt3::util::StandardSubsystems class implementation
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
//  StandardSubsystems::Applications
IMPLEMENT_SINGLETON(StandardSubsystems::Applications)
StandardSubsystems::Applications::Applications() {}
StandardSubsystems::Applications::~Applications() {}

Mnemonic StandardSubsystems::Applications::mnemonic() const
{
    return M(Applications);
}

QString StandardSubsystems::Applications::displayName() const
{
    return "Applications";
}

QString StandardSubsystems::Applications::description() const
{
    return "Top-level applications making up the TimeTracker3";
}

//////////
//  StandardSubsystems::Storage
IMPLEMENT_SINGLETON(StandardSubsystems::Storage)
StandardSubsystems::Storage::Storage() {}
StandardSubsystems::Storage::~Storage() {}

Mnemonic StandardSubsystems::Storage::mnemonic() const
{
    return M(Storage);
}

QString StandardSubsystems::Storage::displayName() const
{
    return "Data storage";
}

QString StandardSubsystems::Storage::description() const
{
    return "Components responsible for persistent data storage";
}

//////////
//  StandardSubsystems::Gui
IMPLEMENT_SINGLETON(StandardSubsystems::Gui)
StandardSubsystems::Gui::Gui() {}
StandardSubsystems::Gui::~Gui() {}

Mnemonic StandardSubsystems::Gui::mnemonic() const
{
    return M(Gui);
}

QString StandardSubsystems::Gui::displayName() const
{
    return "User interface";
}

QString StandardSubsystems::Gui::description() const
{
    return "Components making up the user interface";
}

//////////
//  StandardSubsystems::Reporting
IMPLEMENT_SINGLETON(StandardSubsystems::Reporting)
StandardSubsystems::Reporting::Reporting() {}
StandardSubsystems::Reporting::~Reporting() {}

Mnemonic StandardSubsystems::Reporting::mnemonic() const
{
    return M(Reporting);
}

QString StandardSubsystems::Reporting::displayName() const
{
    return "Reporting";
}

QString StandardSubsystems::Reporting::description() const
{
    return "Components providing summaries and reporting features";
}

//////////
//  StandardSubsystems::Utility
IMPLEMENT_SINGLETON(StandardSubsystems::Utility)
StandardSubsystems::Utility::Utility() {}
StandardSubsystems::Utility::~Utility() {}

Mnemonic StandardSubsystems::Utility::mnemonic() const
{
    return M(Utility);
}

QString StandardSubsystems::Utility::displayName() const
{
    return "Utility";
}

QString StandardSubsystems::Utility::description() const
{
    return "Utility components and support libraries";
}

//  End of tt3-util/StandardSubsystems.cpp
