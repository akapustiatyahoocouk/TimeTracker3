//
//  tt3-db-api/ObjectTypes.cpp tt3::db::api::ObjectTypes class implementation
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
#include "tt3-db-api/API.hpp"
using namespace tt3::db::api;

//////////
//  ObjectTypes::User
IMPLEMENT_SINGLETON(ObjectTypes::User)

ObjectTypes::User::User()
    :   _smallIcon(":/tt3-db-api/Resources/Images/Objects/UserSmall.png"),
        _largeIcon(":/tt3-db-api/Resources/Images/Objects/UserLarge.png")
{
}

ObjectTypes::User::~User()
{
}

QString ObjectTypes::User::mnemonic() const
{
    return "User";
}

QString ObjectTypes::User::displayName() const
{
    return "User";
}

QIcon ObjectTypes::User::smallIcon() const
{
    return _smallIcon;
}

QIcon ObjectTypes::User::largeIcon() const
{
    return _largeIcon;
}

//////////
//  ObjectTypes::Account
IMPLEMENT_SINGLETON(ObjectTypes::Account)

ObjectTypes::Account::Account()
    :   _smallIcon(":/tt3-db-api/Resources/Images/Objects/AccountSmall.png"),
        _largeIcon(":/tt3-db-api/Resources/Images/Objects/AccountLarge.png")
{
}

ObjectTypes::Account::~Account()
{
}

QString ObjectTypes::Account::mnemonic() const
{
    return "Account";
}

QString ObjectTypes::Account::displayName() const
{
    return "Account";
}

QIcon ObjectTypes::Account::smallIcon() const
{
    return _smallIcon;
}

QIcon ObjectTypes::Account::largeIcon() const
{
    return _largeIcon;
}

//  End of tt3-db-api/ObjectTypes.cpp
