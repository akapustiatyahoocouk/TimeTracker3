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

struct MessageDigestManager::_Impl
{
    _Impl()
    {
#define REGISTER(Clazz)                     \
        registry.insert(                    \
            Clazz::instance()->mnemonic(),  \
            Clazz::instance())

        REGISTER(StandardMessageDigests::Sha1);
    }

    Mutex                           guard;
    QMap<Mnemonic, IMessageDigest*> registry;
};

//////////
//  Operations
QSet<IMessageDigest*> MessageDigestManager::allMessageDigests()
{
    _Impl * impl = _impl();
    Lock _(impl->guard);

    QList<IMessageDigest*> values = impl->registry.values();
    return QSet<IMessageDigest*>(values.cbegin(), values.cend());
}

bool MessageDigestManager::registerMessageDigest(IMessageDigest * messageDigest)
{
    Q_ASSERT(messageDigest != nullptr);

    _Impl * impl = _impl();
    Lock _(impl->guard);

    if (impl->registry.contains(messageDigest->mnemonic()))
    {
        return messageDigest == impl->registry[messageDigest->mnemonic()];
    }
    impl->registry[messageDigest->mnemonic()] = messageDigest;
    return true;
}

IMessageDigest * MessageDigestManager::findMessageDigest(const Mnemonic & mnemonic)
{
    _Impl * impl = _impl();
    Lock _(impl->guard);

    return impl->registry.contains(mnemonic) ? impl->registry[mnemonic] : nullptr;
}

//////////
//  Implementation helpers
MessageDigestManager::_Impl * MessageDigestManager::_impl()
{
    static _Impl impl;
    return &impl;
}

//  End of tt3-util/MessageDigestManager.cpp
