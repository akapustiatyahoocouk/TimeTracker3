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
    static Component::Resources *const resources = Component::Resources::instance();   //  idempotent
    return resources->string(RSID(StandardSubsystems), RID(Applications.DisplayName));
}

QString StandardSubsystems::Applications::description() const
{
    static Component::Resources *const resources = Component::Resources::instance();   //  idempotent
    return resources->string(RSID(StandardSubsystems), RID(Applications.Description));
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
    static Component::Resources *const resources = Component::Resources::instance();   //  idempotent
    return resources->string(RSID(StandardSubsystems), RID(Storage.DisplayName));
}

QString StandardSubsystems::Storage::description() const
{
    static Component::Resources *const resources = Component::Resources::instance();   //  idempotent
    return resources->string(RSID(StandardSubsystems), RID(Storage.Description));
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
    static Component::Resources *const resources = Component::Resources::instance();   //  idempotent
    return resources->string(RSID(StandardSubsystems), RID(Gui.DisplayName));
}

QString StandardSubsystems::Gui::description() const
{
    static Component::Resources *const resources = Component::Resources::instance();   //  idempotent
    return resources->string(RSID(StandardSubsystems), RID(Gui.Description));
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
    static Component::Resources *const resources = Component::Resources::instance();   //  idempotent
    return resources->string(RSID(StandardSubsystems), RID(Reporting.DisplayName));
}

QString StandardSubsystems::Reporting::description() const
{
    static Component::Resources *const resources = Component::Resources::instance();   //  idempotent
    return resources->string(RSID(StandardSubsystems), RID(Reporting.Description));
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
    static Component::Resources *const resources = Component::Resources::instance();   //  idempotent
    return resources->string(RSID(StandardSubsystems), RID(Utility.DisplayName));
}

QString StandardSubsystems::Utility::description() const
{
    static Component::Resources *const resources = Component::Resources::instance();   //  idempotent
    return resources->string(RSID(StandardSubsystems), RID(Utility.Description));
}

//  End of tt3-util/StandardSubsystems.cpp
