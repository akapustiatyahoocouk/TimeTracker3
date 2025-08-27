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

tt3::util::Mutex SubsystemManager::_guard;
QMap<QString, ISubsystem*> SubsystemManager::_registry;

//////////
//  Operations
QSet<ISubsystem*> SubsystemManager::allSubsystems()
{
    tt3::util::Lock lock(_guard);

    _registerStandardSubsystems();
    QList<ISubsystem*> values = _registry.values();
    return QSet<ISubsystem*>(values.begin(), values.end());
}

bool SubsystemManager::registerSubsystem(ISubsystem * subsystem)
{
    Q_ASSERT(subsystem != nullptr);

    tt3::util::Lock lock(_guard);

    _registerStandardSubsystems();
    ISubsystem * registeredSubsystem = findSubsystem(subsystem->mnemonic());
    if (registeredSubsystem != nullptr)
    {
        return subsystem == registeredSubsystem;
    }
    else
    {
        QString key = subsystem->mnemonic();
        _registry[key] = subsystem;
        return true;
    }
}

ISubsystem * SubsystemManager::findSubsystem(const QString & mnemonic)
{
    tt3::util::Lock lock(_guard);

    _registerStandardSubsystems();
    QString key = mnemonic;
    return _registry.contains(key) ? _registry[key] : nullptr;
}

//////////
//  Implementation helpers
void SubsystemManager::_registerStandardSubsystems()
{
    Q_ASSERT(_guard.isLockedByCurrentThread());

    if (_registry.isEmpty())
    {
        registerSubsystem(StandardSubsystems::Applications::instance());
        registerSubsystem(StandardSubsystems::Storage::instance());
        registerSubsystem(StandardSubsystems::Gui::instance());
        registerSubsystem(StandardSubsystems::Reporting::instance());
        registerSubsystem(StandardSubsystems::Utility::instance());
    }
}

//  End of tt3-util/MessageDigestManager.cpp
