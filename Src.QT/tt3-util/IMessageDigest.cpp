//
//  tt3-util/IMessageDigest.cpp - The util::IMessageDigest interface implementation
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
//  IMessageDigest::Builder
void IMessageDigest::Builder::digestFragment(const QString & s)
{
    QByteArray bytes = s.toUtf8();
    digestFragment(bytes.data(), bytes.size());
}

QString IMessageDigest::Builder::digestAsString()
{
    return this->digestAsBytes().toHex().toUpper();
}

//  End of tt3-util/IMessageDigest.cpp
