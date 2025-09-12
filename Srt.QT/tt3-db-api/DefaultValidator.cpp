//ina

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
auto DefaultValidator::principal(
    ) const -> IValidator::IPrincipalValidator *
{
    return _PrincipalValidator::instance();
}

auto DefaultValidator::user(
    ) const -> IValidator::IUserValidator *
{
    return _UserValidator::instance();
}

auto DefaultValidator::account(
    ) const -> IValidator::IAccountValidator *
{
    return _AccountValidator::instance();
}

auto DefaultValidator::activityType(
    ) const -> IValidator::IActivityTypeValidator *
{
    return _ActivityTypeValidator::instance();
}

auto DefaultValidator::activity(
    ) const -> IValidator::IActivityValidator *
{
    return _ActivityValidator::instance();
}

auto DefaultValidator::publicActivity(
    ) const -> IValidator::IPublicActivityValidator *
{
    return _PublicActivityValidator::instance();
}

//////////
//  Implementation helpers
bool DefaultValidator::_isValidEmailAddress(
        const QString & emailAddress
    )
{
    static const std::wregex pattern(L"^[a-zA-Z0-9._%+-]+@[a-zA-Z0-9.-]+\\.[a-zA-Z]{2,}$");

    std::wcmatch match;
    std::wstring ws = emailAddress.toStdWString();
    return std::regex_match(ws.c_str(), match, pattern);
}

bool DefaultValidator::_isValidName(
        const QString & name,
        int maxLength
    )
{
    if (name.length() == 0 ||
        name.length() > maxLength ||
        name.length() != name.trimmed().length())
    {   //  OOPS! No good!
        return false;
    }
    for (QChar c : name)
    {
        if (c < '\x20' || c == '\x7F')
        {   //  OOPS! No good!
            return false;
        }
    }
    return true;
}

bool DefaultValidator::_isValidDescription(
        const QString & description,
        int maxLength
    )
{
    if (description.length() > maxLength)
    {   //  OOPS! No good!
        return false;
    }
    for (QChar c : description)
    {
        if ((c < '\x20' && c != '\n' && c != '\t') ||
            c == '\x7F')
        {   //  OOPS! No good!
            return false;
        }
    }
    return true;
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

bool DefaultValidator::_UserValidator::isValidUiLocale(const UiLocale & /*uiLocale*/)
{
    return true;
}

bool DefaultValidator::_UserValidator::isValidRealName(const QString & realName)
{
    return _isValidName(realName, 127);
}

bool DefaultValidator::_UserValidator::isValidInactivityTimeout(const InactivityTimeout & inactivityTimeout)
{
    static tt3::util::TimeSpan Min = tt3::util::TimeSpan::minutes(5);
    static tt3::util::TimeSpan Max = tt3::util::TimeSpan::hours(24);

    return (!inactivityTimeout.has_value() ||
            (inactivityTimeout.value().isValid() &&
             inactivityTimeout.value() >= Min &&
             inactivityTimeout.value() <= Max));
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

//////////
//  DefaultValidator::_ActivityTypeValidator
IMPLEMENT_SINGLETON(DefaultValidator::_ActivityTypeValidator)
DefaultValidator::_ActivityTypeValidator::_ActivityTypeValidator() {}
DefaultValidator::_ActivityTypeValidator::~_ActivityTypeValidator() {}

bool DefaultValidator::_ActivityTypeValidator::isValidDisplayName(
        const QString & displayName
    )
{
    return _isValidName(displayName, 255);
}

bool DefaultValidator::_ActivityTypeValidator::isValidDescription(
        const QString & description
    )
{
    return _isValidDescription(description, 32767);
}

//////////
//  DefaultValidator::_ActivityValidator
IMPLEMENT_SINGLETON(DefaultValidator::_ActivityValidator)
DefaultValidator::_ActivityValidator::_ActivityValidator() {}
DefaultValidator::_ActivityValidator::~_ActivityValidator() {}

bool DefaultValidator::_ActivityValidator::isValidDisplayName(
        const QString & displayName
    )
{
    return _isValidName(displayName, 255);
}

bool DefaultValidator::_ActivityValidator::isValidDescription(
        const QString & description
    )
{
    return _isValidDescription(description, 32767);
}

bool DefaultValidator::_ActivityValidator::isValidTimeout(const InactivityTimeout & timeout)
{
    static tt3::util::TimeSpan Min = tt3::util::TimeSpan::minutes(5);
    static tt3::util::TimeSpan Max = tt3::util::TimeSpan::hours(24);

    return (!timeout.has_value() ||
            (timeout.value().isValid() &&
             timeout.value() >= Min &&
             timeout.value() <= Max));
}

//////////
//  DefaultValidator::_PublicActivityValidator
IMPLEMENT_SINGLETON(DefaultValidator::_PublicActivityValidator)
DefaultValidator::_PublicActivityValidator::_PublicActivityValidator() {}
DefaultValidator::_PublicActivityValidator::~_PublicActivityValidator() {}

bool DefaultValidator::_PublicActivityValidator::isValidDisplayName(
        const QString & displayName
    )
{
    return _ActivityValidator::instance()->isValidDisplayName(displayName);
}

bool DefaultValidator::_PublicActivityValidator::isValidDescription(
        const QString & description
    )
{
    return _ActivityValidator::instance()->isValidDescription(description);
}

bool DefaultValidator::_PublicActivityValidator::isValidTimeout(const InactivityTimeout & timeout)
{
    return _ActivityValidator::instance()->isValidTimeout(timeout);
}

//  End of tt3-db-api/DefaultValidator.cpp
