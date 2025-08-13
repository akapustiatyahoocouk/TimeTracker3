//
//  tt3-util/MessageDigest.cpp - The util::MessageDigest class implementation
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

MessageDigests MessageDigest::_registry;

//////////
//  Registry
MessageDigests MessageDigest::getAll()
{
    return _registry;
}

MessageDigest * MessageDigest::findByMnemonic(const QString & mnemonic)
{
    for (MessageDigest * messageDigest : _registry)
    {
        if (messageDigest->mnemonic() == mnemonic)
        {
            return messageDigest;
        }
    }
    return nullptr;
}

bool MessageDigest::registerMessageDigest(MessageDigest * messageDigest)
{
    Q_ASSERT(messageDigest != nullptr);

    MessageDigest * registeredMessageDigest = findByMnemonic(messageDigest->mnemonic());
    if (registeredMessageDigest == nullptr)
    {
        _registry.insert(messageDigest);
        return true;
    }
    return registeredMessageDigest == messageDigest;
}

//////////
//  MessageDigest::Builder
void MessageDigest::Builder::digest(const QString & s)
{
    QByteArray bytes = s.toUtf8();
    digest(bytes.data(), bytes.size());
}

QString MessageDigest::Builder::digestAsString()
{
    return this->digestAsBytes().toHex().toUpper();
}

//  End of emuone-util/MessageDigest.cpp
