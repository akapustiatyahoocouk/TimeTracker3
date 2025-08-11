//
//  tt3-db-api/DefaultValidator.cpp tt3::db::api::DefaultValidator class implementation
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
//  Singleton
IMPLEMENT_SINGLETON(DefaultValidator)
DefaultValidator::DefaultValidator() {}
DefaultValidator::~DefaultValidator() {}

//////////
//  IValidator
IValidator::IPrincipalValidator * DefaultValidator::principal() const
{
    return _PrincipalValidator::instance();
}

IValidator::IUserValidator * DefaultValidator::user() const
{
    return _UserValidator::instance();
}

IValidator::IAccountValidator * DefaultValidator::account() const
{
    return _AccountValidator::instance();
}

//////////
//  Implementation helpers
bool DefaultValidator::_isValidEmailAddress(const QString & emailAddress)
{
    static const std::wregex pattern(L"^[a-zA-Z0-9._%+-]+@[a-zA-Z0-9.-]+\\.[a-zA-Z]{2,}$");

    std::wcmatch match;
    const wchar_t * wc = emailAddress.toStdWString().c_str();
    return std::regex_match(wc, match, pattern);
}

bool DefaultValidator::_isValidName(const QString & name, int maxLength)
{
    return name.length() > 0 &&
           name.length() <= maxLength &&
           name.length() == name.trimmed().length();
}

//////////
//  DefaultValidator::_PrincipalValidator
IMPLEMENT_SINGLETON(DefaultValidator::_PrincipalValidator)
DefaultValidator::_PrincipalValidator::_PrincipalValidator() {}
DefaultValidator::_PrincipalValidator::~_PrincipalValidator() {}

bool DefaultValidator::_PrincipalValidator::isValidEmailAddress(const QString & emailAddress)
{
    return _isValidEmailAddress(emailAddress);
}

//////////
//  DefaultValidator::_UserValidator
IMPLEMENT_SINGLETON(DefaultValidator::_UserValidator)
DefaultValidator::_UserValidator::_UserValidator() {}
DefaultValidator::_UserValidator::~_UserValidator() {}

bool DefaultValidator::_UserValidator::isValidEmailAddress(const QString & emailAddress)
{
    return _isValidEmailAddress(emailAddress);
}

bool DefaultValidator::_UserValidator::isValidRealName(const QString & realName)
{
    return _isValidName(realName, 127);
}

bool DefaultValidator::_UserValidator::isValidInactivityTimeout(const tt3::util::TimeSpan & inactivityTimeout)
{
    static tt3::util::TimeSpan Min = tt3::util::TimeSpan::minutes(5);
    static tt3::util::TimeSpan Max = tt3::util::TimeSpan::hours(24);

    return inactivityTimeout >= Min && inactivityTimeout <= Max;
}

//////////
//  DefaultValidator::_AccountValidator
IMPLEMENT_SINGLETON(DefaultValidator::_AccountValidator)
DefaultValidator::_AccountValidator::_AccountValidator() {}
DefaultValidator::_AccountValidator::~_AccountValidator() {}

bool DefaultValidator::_AccountValidator::isValidEmailAddress(const QString & emailAddress)
{
    return _isValidEmailAddress(emailAddress);
}

bool DefaultValidator::_AccountValidator::isValidLogin(const QString & login)
{
    return _isValidName(login, 127);
}

bool DefaultValidator::_AccountValidator::isValidPassword(const QString & /*password*/)
{
    return true;
}

//  End of tt3-db-api/DefaultValidator.cpp
