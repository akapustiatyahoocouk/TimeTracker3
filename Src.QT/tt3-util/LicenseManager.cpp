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

struct LicenseManager::_Impl
{
    _Impl()
    {
        for (auto license : StandardLicenses::all())
        {
            Q_ASSERT(!registry.contains(license->mnemonic()));
            registry[license->mnemonic()] = license;
        }
    }

    using Registry = QMap<Mnemonic, ILicense*>;

    Mutex       guard;
    Registry    registry;
};

//////////
//  Operations
Licenses LicenseManager::all()
{
    _Impl * impl = _impl();
    tt3::util::Lock _(impl->guard);

    QList<ILicense*> values = impl->registry.values();
    return Licenses(values.cbegin(), values.cend());
}

bool LicenseManager::register(ILicense * license)
{
    Q_ASSERT(license != nullptr);

    _Impl * impl = _impl();
    tt3::util::Lock _(impl->guard);

    if (impl->registry.contains(license->mnemonic()))
    {
        return license == impl->registry[license->mnemonic()];
    }
    impl->registry[license->mnemonic()] = license;
    return true;
}

ILicense * LicenseManager::find(
        const Mnemonic & mnemonic
    )
{
    _Impl * impl = _impl();
    tt3::util::Lock _(impl->guard);

    return impl->registry.contains(mnemonic) ? impl->registry[mnemonic] : nullptr;
}

//////////
//  Implementation helpers
LicenseManager::_Impl * LicenseManager::_impl()
{
    static _Impl impl;
    return &impl;
}

//  End of tt3-util/MessageDigestManager.cpp
