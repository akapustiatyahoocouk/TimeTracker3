//
//  tt3-util/LicenseManager.cpp - tt3::util::LicenseManager class implementation
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

Mutex LicenseManager::_guard;
QMap<Mnemonic, ILicense*> LicenseManager::_registry;

//////////
//  Operations
Licenses LicenseManager::allLicenses()
{
    tt3::util::Lock lock(_guard);

    _registerStandardLicenses();
    QList<ILicense*> values = _registry.values();
    return Licenses(values.begin(), values.end());
}

bool LicenseManager::registerLicense(ILicense * license)
{
    Q_ASSERT(license != nullptr);

    tt3::util::Lock lock(_guard);

    _registerStandardLicenses();
    if (ILicense * registeredLicense = findLicense(license->mnemonic()))
    {
        return license == registeredLicense;
    }
    _registry[license->mnemonic()] = license;
    return true;
}

ILicense * LicenseManager::findLicense(const Mnemonic & mnemonic)
{
    Lock lock(_guard);

    _registerStandardLicenses();
    return _registry.contains(mnemonic) ? _registry[mnemonic] : nullptr;
}

//////////
//  Implementation helpers
void LicenseManager::_registerStandardLicenses()
{
    Q_ASSERT(_guard.isLockedByCurrentThread());

    if (_registry.isEmpty())
    {
#define REGISTER(Subsystem)                                         \
        _registry.insert(                                           \
            StandardLicenses::Subsystem::instance()->mnemonic(),    \
            StandardLicenses::Subsystem::instance())
        REGISTER(Gpl3);
    }
}

//  End of tt3-util/MessageDigestManager.cpp
