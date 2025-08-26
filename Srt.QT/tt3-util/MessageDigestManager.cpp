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

tt3::util::Mutex MessageDigestManager::_guard;
QMap<QString, IMessageDigest*> MessageDigestManager::_registry;

//////////
//  Operations
QSet<IMessageDigest*> MessageDigestManager::allMessageDigests()
{
    tt3::util::Lock lock(_guard);

    _registerStandardMessageDigests();
    QList<IMessageDigest*> values = _registry.values();
    return QSet<IMessageDigest*>(values.begin(), values.end());
}

bool MessageDigestManager::registerMessageDigest(IMessageDigest * messageDigest)
{
    Q_ASSERT(messageDigest != nullptr);

    tt3::util::Lock lock(_guard);

    _registerStandardMessageDigests();
    IMessageDigest * registeredMessageDigest = findMessageDigest(messageDigest->mnemonic());
    if (registeredMessageDigest != nullptr)
    {
        return messageDigest == registeredMessageDigest;
    }
    else
    {
        QString key = messageDigest->mnemonic();
        _registry[key] = messageDigest;
        return true;
    }
}

IMessageDigest * MessageDigestManager::findMessageDigest(const QString & mnemonic)
{
    tt3::util::Lock lock(_guard);

    _registerStandardMessageDigests();
    QString key = mnemonic;
    return _registry.contains(key) ? _registry[key] : nullptr;
}

//////////
//  Implementation helpers
void MessageDigestManager::_registerStandardMessageDigests()
{
    Q_ASSERT(_guard.isLockedByCurrentThread());

    registerMessageDigest(Sha1MessageDigest::instance());
}

//  End of tt3-util/MessageDigestManager.cpp
