//
//  tt3-util/MessageDigestManager.cpp - tt3::util::MessageDigestManager class implementation
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

Mutex MessageDigestManager::_guard;
QMap<Mnemonic, IMessageDigest*> MessageDigestManager::_registry;

//////////
//  Operations
QSet<IMessageDigest*> MessageDigestManager::allMessageDigests()
{
    Lock lock(_guard);

    _registerStandardMessageDigests();
    QList<IMessageDigest*> values = _registry.values();
    return QSet<IMessageDigest*>(values.begin(), values.end());
}

bool MessageDigestManager::registerMessageDigest(IMessageDigest * messageDigest)
{
    Q_ASSERT(messageDigest != nullptr);

    Lock lock(_guard);

    _registerStandardMessageDigests();
    if (IMessageDigest * registeredMessageDigest = findMessageDigest(messageDigest->mnemonic()))
    {
        return messageDigest == registeredMessageDigest;
    }
    _registry[messageDigest->mnemonic()] = messageDigest;
    return true;
}

IMessageDigest * MessageDigestManager::findMessageDigest(const Mnemonic & mnemonic)
{
    Lock lock(_guard);

    _registerStandardMessageDigests();
    return _registry.contains(mnemonic) ? _registry[mnemonic] : nullptr;
}

//////////
//  Implementation helpers
void MessageDigestManager::_registerStandardMessageDigests()
{
    Q_ASSERT(_guard.isLockedByCurrentThread());

    if (_registry.isEmpty())
    {
#define REGISTER(Clazz)                                 \
        _registry.insert(Clazz::instance()->mnemonic(), \
                         Clazz::instance())
        REGISTER(Sha1MessageDigest);
    }
}

//  End of tt3-util/MessageDigestManager.cpp
