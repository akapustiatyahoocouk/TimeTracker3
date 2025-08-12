//
//  tt3-db-api/DatabaseObjectTypes.cpp tt3::db::api::DatabaseObjectTypes class implementation
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
//  DatabaseObjectTypes::User
IMPLEMENT_SINGLETON(DatabaseObjectTypes::User)

DatabaseObjectTypes::User::User()
    :   _smallIcon(":/tt3-db-api/Resources/Images/Objects/UserSmall.png"),
        _largeIcon(":/tt3-db-api/Resources/Images/Objects/UserLarge.png")
{
}

DatabaseObjectTypes::User::~User()
{
}

QString DatabaseObjectTypes::User::mnemonic() const
{
    return "User";
}

QString DatabaseObjectTypes::User::displayName() const
{
    return "User";
}

QIcon DatabaseObjectTypes::User::smallIcon() const
{
    return _smallIcon;
}

QIcon DatabaseObjectTypes::User::largeIcon() const
{
    return _largeIcon;
}

//////////
//  DatabaseObjectTypes::Account
IMPLEMENT_SINGLETON(DatabaseObjectTypes::Account)

DatabaseObjectTypes::Account::Account()
    :   _smallIcon(":/tt3-db-api/Resources/Images/Objects/AccountSmall.png"),
        _largeIcon(":/tt3-db-api/Resources/Images/Objects/AccountLarge.png")
{
}

DatabaseObjectTypes::Account::~Account()
{
}

QString DatabaseObjectTypes::Account::mnemonic() const
{
    return "Account";
}

QString DatabaseObjectTypes::Account::displayName() const
{
    return "Account";
}

QIcon DatabaseObjectTypes::Account::smallIcon() const
{
    return _smallIcon;
}

QIcon DatabaseObjectTypes::Account::largeIcon() const
{
    return _largeIcon;
}

//  End of tt3-db-api/DatabaseObjectTypes.cpp
