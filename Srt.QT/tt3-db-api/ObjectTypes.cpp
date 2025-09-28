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
ObjectTypes::User::User() {}
ObjectTypes::User::~User() {}

auto ObjectTypes::User::mnemonic(
    ) const -> tt3::util::Mnemonic
{
    return M(User);
}

QString ObjectTypes::User::displayName() const
{
    return "User";
}

QIcon ObjectTypes::User::smallIcon() const
{
    static QIcon icon(":/tt3-db-api/Resources/Images/Objects/UserSmall.png");
    return icon;
}

QIcon ObjectTypes::User::largeIcon() const
{
    static QIcon icon(":/tt3-db-api/Resources/Images/Objects/UserLarge.png");
    return icon;
}

//////////
//  ObjectTypes::Account
IMPLEMENT_SINGLETON(ObjectTypes::Account)
ObjectTypes::Account::Account() {}
ObjectTypes::Account::~Account() {}

auto ObjectTypes::Account::mnemonic(
    ) const -> tt3::util::Mnemonic
{
    return M(Account);
}

QString ObjectTypes::Account::displayName() const
{
    return "Account";
}

QIcon ObjectTypes::Account::smallIcon() const
{
    static QIcon icon(":/tt3-db-api/Resources/Images/Objects/AccountSmall.png");
    return icon;
}

QIcon ObjectTypes::Account::largeIcon() const
{
    static QIcon icon(":/tt3-db-api/Resources/Images/Objects/AccountLarge.png");
    return icon;
}

//////////
//  ObjectTypes::ActivityType
IMPLEMENT_SINGLETON(ObjectTypes::ActivityType)
ObjectTypes::ActivityType::ActivityType() {}
ObjectTypes::ActivityType::~ActivityType() {}

auto ObjectTypes::ActivityType::mnemonic(
    ) const -> tt3::util::Mnemonic
{
    return M(ActivityType);
}

QString ObjectTypes::ActivityType::displayName() const
{
    return "Activity type";
}

QIcon ObjectTypes::ActivityType::smallIcon() const
{
    static QIcon icon(":/tt3-db-api/Resources/Images/Objects/ActivityTypeSmall.png");
    return icon;
}

QIcon ObjectTypes::ActivityType::largeIcon() const
{
    static QIcon icon(":/tt3-db-api/Resources/Images/Objects/ActivityTypeLarge.png");
    return icon;
}

//////////
//  ObjectTypes::PublicActivity
IMPLEMENT_SINGLETON(ObjectTypes::PublicActivity)
ObjectTypes::PublicActivity::PublicActivity() {}
ObjectTypes::PublicActivity::~PublicActivity() {}

auto ObjectTypes::PublicActivity::mnemonic(
    ) const -> tt3::util::Mnemonic
{
    return M(PublicActivity);
}

QString ObjectTypes::PublicActivity::displayName() const
{
    return "Public activity";
}

QIcon ObjectTypes::PublicActivity::smallIcon() const
{
    static QIcon icon(":/tt3-db-api/Resources/Images/Objects/PublicActivitySmall.png");
    return icon;
}

QIcon ObjectTypes::PublicActivity::largeIcon() const
{
    static QIcon icon(":/tt3-db-api/Resources/Images/Objects/PublicActivityLarge.png");
    return icon;
}

//////////
//  ObjectTypes::PublicTask
IMPLEMENT_SINGLETON(ObjectTypes::PublicTask)
ObjectTypes::PublicTask::PublicTask() {}
ObjectTypes::PublicTask::~PublicTask() {}

auto ObjectTypes::PublicTask::mnemonic(
    ) const -> tt3::util::Mnemonic
{
    return M(PublicTask);
}

QString ObjectTypes::PublicTask::displayName() const
{
    return "Public task";
}

QIcon ObjectTypes::PublicTask::smallIcon() const
{
    static QIcon icon(":/tt3-db-api/Resources/Images/Objects/PublicTaskSmall.png");
    return icon;
}

QIcon ObjectTypes::PublicTask::largeIcon() const
{
    static QIcon icon(":/tt3-db-api/Resources/Images/Objects/PublicTaskLarge.png");
    return icon;
}

//////////
//  ObjectTypes::PrivateActivity
IMPLEMENT_SINGLETON(ObjectTypes::PrivateActivity)
ObjectTypes::PrivateActivity::PrivateActivity() {}
ObjectTypes::PrivateActivity::~PrivateActivity() {}

auto ObjectTypes::PrivateActivity::mnemonic(
    ) const -> tt3::util::Mnemonic
{
    return M(PrivateActivity);
}

QString ObjectTypes::PrivateActivity::displayName() const
{
    return "Private activity";
}

QIcon ObjectTypes::PrivateActivity::smallIcon() const
{
    static QIcon icon(":/tt3-db-api/Resources/Images/Objects/PrivateActivitySmall.png");
    return icon;
}

QIcon ObjectTypes::PrivateActivity::largeIcon() const
{
    static QIcon icon(":/tt3-db-api/Resources/Images/Objects/PrivateActivityLarge.png");
    return icon;
}

//////////
//  ObjectTypes::PrivateTask
IMPLEMENT_SINGLETON(ObjectTypes::PrivateTask)
ObjectTypes::PrivateTask::PrivateTask() {}
ObjectTypes::PrivateTask::~PrivateTask() {}

auto ObjectTypes::PrivateTask::mnemonic(
    ) const -> tt3::util::Mnemonic
{
    return M(PrivateTask);
}

QString ObjectTypes::PrivateTask::displayName() const
{
    return "Private task";
}

QIcon ObjectTypes::PrivateTask::smallIcon() const
{
    static QIcon icon(":/tt3-db-api/Resources/Images/Objects/PrivateTaskSmall.png");
    return icon;
}

QIcon ObjectTypes::PrivateTask::largeIcon() const
{
    static QIcon icon(":/tt3-db-api/Resources/Images/Objects/PrivateTaskLarge.png");
    return icon;
}

//////////
//  ObjectTypes::Work
IMPLEMENT_SINGLETON(ObjectTypes::Work)
ObjectTypes::Work::Work() {}
ObjectTypes::Work::~Work() {}

auto ObjectTypes::Work::mnemonic(
    ) const -> tt3::util::Mnemonic
{
    return M(Work);
}

QString ObjectTypes::Work::displayName() const
{
    return "Work";
}

QIcon ObjectTypes::Work::smallIcon() const
{
    static QIcon icon(":/tt3-db-api/Resources/Images/Objects/WorkSmall.png");
    return icon;
}

QIcon ObjectTypes::Work::largeIcon() const
{
    static QIcon icon(":/tt3-db-api/Resources/Images/Objects/WorkLarge.png");
    return icon;
}

//////////
//  ObjectTypes::Event
IMPLEMENT_SINGLETON(ObjectTypes::Event)
ObjectTypes::Event::Event() {}
ObjectTypes::Event::~Event() {}

auto ObjectTypes::Event::mnemonic(
    ) const -> tt3::util::Mnemonic
{
    return M(Event);
}

QString ObjectTypes::Event::displayName() const
{
    return "Event";
}

QIcon ObjectTypes::Event::smallIcon() const
{
    static QIcon icon(":/tt3-db-api/Resources/Images/Objects/EventSmall.png");
    return icon;
}

QIcon ObjectTypes::Event::largeIcon() const
{
    static QIcon icon(":/tt3-db-api/Resources/Images/Objects/EventLarge.png");
    return icon;
}

//////////
//  ObjectTypes::Project
IMPLEMENT_SINGLETON(ObjectTypes::Project)
ObjectTypes::Project::Project() {}
ObjectTypes::Project::~Project() {}

auto ObjectTypes::Project::mnemonic(
    ) const -> tt3::util::Mnemonic
{
    return M(Project);
}

QString ObjectTypes::Project::displayName() const
{
    return "Project";
}

QIcon ObjectTypes::Project::smallIcon() const
{
    static QIcon icon(":/tt3-db-api/Resources/Images/Objects/ProjectSmall.png");
    return icon;
}

QIcon ObjectTypes::Project::largeIcon() const
{
    static QIcon icon(":/tt3-db-api/Resources/Images/Objects/ProjectLarge.png");
    return icon;
}

//////////
//  ObjectTypes::WorkStream
IMPLEMENT_SINGLETON(ObjectTypes::WorkStream)
ObjectTypes::WorkStream::WorkStream() {}
ObjectTypes::WorkStream::~WorkStream() {}

auto ObjectTypes::WorkStream::mnemonic(
    ) const -> tt3::util::Mnemonic
{
    return M(WorkStream);
}

QString ObjectTypes::WorkStream::displayName() const
{
    return "Work stream";
}

QIcon ObjectTypes::WorkStream::smallIcon() const
{
    static QIcon icon(":/tt3-db-api/Resources/Images/Objects/WorkStreamSmall.png");
    return icon;
}

QIcon ObjectTypes::WorkStream::largeIcon() const
{
    static QIcon icon(":/tt3-db-api/Resources/Images/Objects/WorkStreamLarge.png");
    return icon;
}

//////////
//  ObjectTypes::Beneficiary
IMPLEMENT_SINGLETON(ObjectTypes::Beneficiary)
ObjectTypes::Beneficiary::Beneficiary() {}
ObjectTypes::Beneficiary::~Beneficiary() {}

auto ObjectTypes::Beneficiary::mnemonic(
    ) const -> tt3::util::Mnemonic
{
    return M(Beneficiary);
}

QString ObjectTypes::Beneficiary::displayName() const
{
    return "Beneficiary";
}

QIcon ObjectTypes::Beneficiary::smallIcon() const
{
    static QIcon icon(":/tt3-db-api/Resources/Images/Objects/BeneficiarySmall.png");
    return icon;
}

QIcon ObjectTypes::Beneficiary::largeIcon() const
{
    static QIcon icon(":/tt3-db-api/Resources/Images/Objects/BeneficiaryLarge.png");
    return icon;
}

//  End of tt3-db-api/ObjectTypes.cpp
