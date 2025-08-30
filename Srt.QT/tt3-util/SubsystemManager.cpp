//
//  tt3-util/SubsystemManager.cpp - tt3::util::SubsystemManager class implementation
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

Mutex SubsystemManager::_guard;
QMap<Mnemonic, ISubsystem*> SubsystemManager::_registry;

//////////
//  Operations
Subsystems SubsystemManager::allSubsystems()
{
    Lock lock(_guard);

    _registerStandardSubsystems();
    QList<ISubsystem*> values = _registry.values();
    return Subsystems(values.begin(), values.end());
}

bool SubsystemManager::registerSubsystem(ISubsystem * subsystem)
{
    Q_ASSERT(subsystem != nullptr);

    Lock lock(_guard);

    _registerStandardSubsystems();
    if (ISubsystem * registeredSubsystem = findSubsystem(subsystem->mnemonic()))
    {
        return subsystem == registeredSubsystem;
    }
    _registry[subsystem->mnemonic()] = subsystem;
    return true;
}

ISubsystem * SubsystemManager::findSubsystem(const Mnemonic & mnemonic)
{
    Lock lock(_guard);

    _registerStandardSubsystems();
    return _registry.contains(mnemonic) ? _registry[mnemonic] : nullptr;
}

//////////
//  Implementation helpers
void SubsystemManager::_registerStandardSubsystems()
{
    Q_ASSERT(_guard.isLockedByCurrentThread());

    if (_registry.isEmpty())
    {
#define REGISTER(Subsystem)                                         \
        _registry.insert(                                           \
            StandardSubsystems::Subsystem::instance()->mnemonic(),  \
            StandardSubsystems::Subsystem::instance())
        REGISTER(Applications);
        REGISTER(Storage);
        REGISTER(Gui);
        REGISTER(Reporting);
        REGISTER(Utility);
    }
}

//  End of tt3-util/MessageDigestManager.cpp
