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
    return _ActivityValidator::instance();
}

auto DefaultValidator::privateActivity(
    ) const -> IValidator::IPrivateActivityValidator *
{
    return _ActivityValidator::instance();
}

auto DefaultValidator::task(
    ) const -> IValidator::ITaskValidator *
{
    return _ActivityValidator::instance();
}

auto DefaultValidator::publicTask(
    ) const -> IPublicTaskValidator *
{
    return _ActivityValidator::instance();
}

auto DefaultValidator::privateTask(
    ) const -> IPrivateTaskValidator *
{
    return _ActivityValidator::instance();
}

auto DefaultValidator::workload(
    ) const -> IValidator::IWorkloadValidator *
{
    return _WorkloadValidator::instance();
}

auto DefaultValidator::project(
    ) const -> IProjectValidator *
{
    return _WorkloadValidator::instance();
}

auto DefaultValidator::workStream(
    ) const -> IWorkStreamValidator *
{
    return _WorkloadValidator::instance();
}

auto DefaultValidator::beneficiary(
    ) const -> IBeneficiaryValidator *
{
    return _BeneficiaryValidator::instance();
}

auto DefaultValidator::work(
    ) const -> IWorkValidator *
{
    return _WorkValidator::instance();
}

auto DefaultValidator::event(
    ) const -> IEventValidator *
{
    return _EventValidator::instance();
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

bool DefaultValidator::_isValidDateTime(
        const QDateTime & dateTime
    )
{
    static const QCalendar gregorianCalendar(QCalendar::System::Gregorian);

    Q_ASSERT(dateTime.timeSpec() == Qt::TimeSpec::UTC);
    QDate date = dateTime.date();
    QTime time = dateTime.time();
    int y = date.year(gregorianCalendar),
        m = date.month(gregorianCalendar),
        d = date.day(gregorianCalendar);
    int h = time.hour(),
        n = time.minute(),
        s = time.second(),
        l = time.msec();
    return dateTime.isValid() &&
           y >= 1970 && y <= 9999 &&
           m >= 1 && m <= 12 &&
           d >= 1 && d <= 31 &&
           h >= 0 && h <= 23 &&
           n >= 0 && n <= 59 &&
           s >= 0 && s <= 59 && //  QDateTime doesn't do UTC leap seconds!
           l >= 0 && l <= 999;
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
    return _isValidName(displayName, 127);
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
    return _isValidName(displayName, 127);
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
//  DefaultValidator::_WorkloadValidator
IMPLEMENT_SINGLETON(DefaultValidator::_WorkloadValidator)
DefaultValidator::_WorkloadValidator::_WorkloadValidator() {}
DefaultValidator::_WorkloadValidator::~_WorkloadValidator() {}

bool DefaultValidator::_WorkloadValidator::isValidDisplayName(
        const QString & displayName
    )
{
    return _isValidName(displayName, 127);
}

bool DefaultValidator::_WorkloadValidator::isValidDescription(
        const QString & description
    )
{
    return _isValidDescription(description, 32767);
}

//////////
//  DefaultValidator::_BeneficiaryValidator
IMPLEMENT_SINGLETON(DefaultValidator::_BeneficiaryValidator)
DefaultValidator::_BeneficiaryValidator::_BeneficiaryValidator() {}
DefaultValidator::_BeneficiaryValidator::~_BeneficiaryValidator() {}

bool DefaultValidator::_BeneficiaryValidator::isValidDisplayName(
        const QString & displayName
    )
{
    return _isValidName(displayName, 127);
}

bool DefaultValidator::_BeneficiaryValidator::isValidDescription(
        const QString & description
    )
{
    return _isValidDescription(description, 32767);
}

//////////
//  DefaultValidator::_WorkValidator
IMPLEMENT_SINGLETON(DefaultValidator::_WorkValidator)
DefaultValidator::_WorkValidator::_WorkValidator() {}
DefaultValidator::_WorkValidator::~_WorkValidator() {}

bool DefaultValidator::_WorkValidator::isValidStartedFinishedAt(
        const QDateTime & startedAt,
        const QDateTime & finishedAt
    )
{
    return _isValidDateTime(startedAt) &&
           _isValidDateTime(finishedAt) &&
           startedAt <= finishedAt;
}

//////////
//  DefaultValidator::_EventValidator
IMPLEMENT_SINGLETON(DefaultValidator::_EventValidator)
DefaultValidator::_EventValidator::_EventValidator() {}
DefaultValidator::_EventValidator::~_EventValidator() {}

bool DefaultValidator::_EventValidator::isValidOccurredAt(
        const QDateTime & occurredAt
    )
{
    return _isValidDateTime(occurredAt);
}

bool DefaultValidator::_EventValidator::isValidComment(
        const QString & comment
    )
{
    return _isValidName(comment, 255);
}

//  End of tt3-db-api/DefaultValidator.cpp
