//
//  tt3-ws/ActivityImpl.cpp - tt3::ws::ActivityImpl class implementation
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
#include "tt3-ws/API.hpp"
    using namespace tt3::ws;

//////////
//  Construction/destruction
ActivityImpl::ActivityImpl(
        Workspace workspace,
        tt3::db::api::IActivity * dataActivity
    ) : ObjectImpl(workspace, dataActivity),
        _dataActivity(dataActivity)
{
    _dataActivity->addReference();
}

ActivityImpl::~ActivityImpl()
{
    _dataActivity->removeReference();
}

//////////
//  Operations (properties)
auto ActivityImpl::displayName(
        const Credentials & /*credentials*/
    ) const -> QString
{
    throw tt3::util::NotImplementedError();
}

void ActivityImpl::setDisplayName(
        const Credentials & /*credentials*/,
        const QString & /*displayName*/
    )
{
    throw tt3::util::NotImplementedError();
}

auto ActivityImpl::description(
        const Credentials & /*credentials*/
    ) const -> QString
{
    throw tt3::util::NotImplementedError();
}

void ActivityImpl::setDescription(
        const Credentials & /*credentials*/,
        const QString & /*description*/
    )
{
    throw tt3::util::NotImplementedError();
}

auto ActivityImpl::timeout(
        const Credentials & /*credentials*/
    ) const -> InactivityTimeout
{
    throw tt3::util::NotImplementedError();
}

void ActivityImpl::setTimeout(
        const Credentials & /*credentials*/,
        const InactivityTimeout & /*timeout*/
    )
{
    throw tt3::util::NotImplementedError();
}

bool ActivityImpl::requireCommentOnStart(
        const Credentials & /*credentials*/
    ) const
{
    throw tt3::util::NotImplementedError();
}

void ActivityImpl::setRequireCommentOnStart(
        const Credentials & /*credentials*/,
        bool /*requireCommentOnStart*/
    )
{
    throw tt3::util::NotImplementedError();
}

bool ActivityImpl::requireCommentOnFinish(
        const Credentials & /*credentials*/
    ) const
{
    throw tt3::util::NotImplementedError();
}

void ActivityImpl::setRequireCommentOnFinish(
        const Credentials & /*credentials*/,
        bool /*requireCommentOnFinish*/
    )
{
    throw tt3::util::NotImplementedError();
}

bool ActivityImpl::fullScreenReminder(
        const Credentials & /*credentials*/
    ) const
{
    throw tt3::util::NotImplementedError();
}

void ActivityImpl::setFullScreenReminder(
        const Credentials & /*credentials*/,
        bool /*fullScreenReminder*/
    )
{
    throw tt3::util::NotImplementedError();
}

//////////
//  Operations (associations)
auto ActivityImpl::activityType(
        const Credentials & /*credentials*/
    ) const -> ActivityType
{
    throw tt3::util::NotImplementedError();
}

void ActivityImpl::setActivityType(
        const Credentials & /*credentials*/,
        ActivityType /*activityType*/
    )
{
    throw tt3::util::NotImplementedError();
}

auto ActivityImpl::workload(
        const Credentials & /*credentials*/
    ) const -> Workload
{
    throw tt3::util::NotImplementedError();
}

void ActivityImpl::setWorkload(
        const Credentials & /*credentials*/,
        Workload /*workload*/
    )
{
    throw tt3::util::NotImplementedError();
}

auto ActivityImpl::works(
        const Credentials & /*credentials*/
    ) const -> Works
{
    throw tt3::util::NotImplementedError();
}

auto ActivityImpl::events(
        const Credentials & /*credentials*/
    ) const -> Events
{
    throw tt3::util::NotImplementedError();
}

//  End of tt3-ws/ActivityImpl.cpp
