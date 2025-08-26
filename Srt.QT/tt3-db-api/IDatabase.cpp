//
//  tt3-db-api/IDatabase.cpp tt3::db::api::IDatabase interface implementation
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
//  Operations (access control)
IAccount * IDatabase::login(const QString & login, const QString & password) const throws(DatabaseException)
{
    if (IAccount * account = tryLogin(login, password))
    {
        return account;
    }
    throw AccessDeniedException();
}

//  End of tt3-db-api/IDatabase.cpp
