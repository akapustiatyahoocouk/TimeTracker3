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

struct SubsystemManager::_Impl
{
    _Impl()
    {
#define REGISTER(Subsystem)                     \
        registry.insert(                        \
            Subsystem::instance()->mnemonic(),  \
            Subsystem::instance())

        REGISTER(StandardSubsystems::Applications);
        REGISTER(StandardSubsystems::Storage);
        REGISTER(StandardSubsystems::Gui);
        REGISTER(StandardSubsystems::Reporting);
        REGISTER(StandardSubsystems::Utility);
    }

    Mutex                       guard;
    QMap<Mnemonic, ISubsystem*> registry;
};

//////////
//  Operations
Subsystems SubsystemManager::allSubsystems()
{
    _Impl * impl = _impl();
    Lock _(impl->guard);

    QList<ISubsystem*> values = impl->registry.values();
    return Subsystems(values.cbegin(), values.cend());
}

bool SubsystemManager::registerSubsystem(ISubsystem * subsystem)
{
    Q_ASSERT(subsystem != nullptr);

    _Impl * impl = _impl();
    Lock _(impl->guard);

    if (impl->registry.contains(subsystem->mnemonic()))
    {
        return subsystem == impl->registry[subsystem->mnemonic()];
    }
    impl->registry[subsystem->mnemonic()] = subsystem;
    return true;
}

ISubsystem * SubsystemManager::findSubsystem(const Mnemonic & mnemonic)
{
    _Impl * impl = _impl();
    Lock _(impl->guard);

    return impl->registry.contains(mnemonic) ? impl->registry[mnemonic] : nullptr;
}

//////////
//  Implementation helpers
SubsystemManager::_Impl * SubsystemManager::_impl()
{
    static _Impl impl;
    return &impl;
}

//  End of tt3-util/MessageDigestManager.cpp
