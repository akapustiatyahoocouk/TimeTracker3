//
//  tt3-ws/CurrentActivity.cpp - tt3::gui::CurrentActivity class implementation
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

namespace tt3::ws
{
    extern CurrentCredentials theCurrentCredentials;
}

struct CurrentActivity::_Impl
{
    std::atomic<int>    instanceCount = 0;  //  ...to disallow a second instance
    tt3::util::Mutex    guard;
    Activity            activity = nullptr;
    QDateTime           lastChangedAt = QDateTime::currentDateTimeUtc();
};

//////////
//  Construction/destruction
CurrentActivity::CurrentActivity()
{
    _Impl * impl = _impl();
    Q_ASSERT(impl->instanceCount == 0);
    impl->instanceCount++;
}

CurrentActivity::~CurrentActivity()
{
    _Impl * impl = _impl();
    Q_ASSERT(impl->instanceCount == 1);
    impl->instanceCount--;
}

//////////
//  Operators
void CurrentActivity::operator = (const Activity & activity)
{
    try
    {
        replace(activity, theCurrentCredentials);
    }
    catch (const tt3::util::Exception & ex)
    {
        qCritical() << ex.errorMessage();
    }
}

Activity CurrentActivity::operator -> () const
{
    _Impl * impl = _impl();
    tt3::util::Lock lock(impl->guard);
    Q_ASSERT(impl->instanceCount == 1);

    return impl->activity;
}

CurrentActivity::operator Activity() const
{
    _Impl * impl = _impl();
    tt3::util::Lock lock(impl->guard);
    Q_ASSERT(impl->instanceCount == 1);

    return impl->activity;
}

bool CurrentActivity::operator == (nullptr_t /*null*/) const
{
    _Impl * impl = _impl();
    tt3::util::Lock lock(impl->guard);
    Q_ASSERT(impl->instanceCount == 1);

    return impl->activity.get() == nullptr;
}

bool CurrentActivity::operator != (nullptr_t /*null*/) const
{
    _Impl * impl = _impl();
    tt3::util::Lock lock(impl->guard);
    Q_ASSERT(impl->instanceCount == 1);

    return impl->activity.get() != nullptr;
}

bool CurrentActivity::operator == (Activity activity) const
{
    _Impl * impl = _impl();
    tt3::util::Lock lock(impl->guard);
    Q_ASSERT(impl->instanceCount == 1);

    return impl->activity == activity;
}

bool CurrentActivity::operator != (Activity activity) const
{
    _Impl * impl = _impl();
    tt3::util::Lock lock(impl->guard);
    Q_ASSERT(impl->instanceCount == 1);

    return impl->activity != activity;
}

//////////
//  Operations
QDateTime CurrentActivity::lastChangedAt() const
{
    _Impl * impl = _impl();
    tt3::util::Lock lock(impl->guard);
    Q_ASSERT(impl->instanceCount == 1);

    return impl->lastChangedAt;
}

bool CurrentActivity::replaceWoth(
        Activity with,
        const Credentials & credentials
    )
{
    Activity before, after;

    //  Change is effected in a "locked" state
    {
        _Impl * impl = _impl();
        tt3::util::Lock lock(impl->guard);
        Q_ASSERT(impl->instanceCount == 1);

        if (with != impl->activity)
        {   //  Enter the comment if necessary
            bool commentRequired =
                (impl->activity != nullptr &&
                 impl->activity->requireCommentOnFinish(credentials)) ||    //  may throw
                (with != nullptr &&
                 with->requireCommentOnStart(credentials)); //  may throw
            //  Log current activity, if there is one, as a Work item
            if (impl->activity != nullptr)
            {   //  TODO properly
                qDebug() << "Logging "
                         << impl->activity->displayName(credentials)
                         << " from "
                         << impl->lastChangedAt
                         << " to "
                         << QDateTime::currentDateTimeUtc();
            }
            //  TODO Log entered comment as an Event
            //  Make the change
            before = impl->activity;
            impl->activity = with;
            impl->lastChangedAt = QDateTime::currentDateTimeUtc();
            after = impl->activity;
        }
    }
    //  Signal is sent in a "not locked" state
    if (before != after)
    {
        emit changed(before, after);
        return true;
    }
    return false;
}

//////////
//  Implementation helpers
auto CurrentActivity::_impl()
    -> CurrentActivity::_Impl *
{
    static _Impl impl;
    return &impl;
}

//////////
//  Global statics
namespace tt3::ws
{
    Q_DECL_EXPORT CurrentActivity theCurrentActivity;
}

//  End of tt3-ws/CurrentWorkspace.cpp
