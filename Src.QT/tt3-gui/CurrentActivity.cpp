//
//  tt3-gui/CurrentActivity.cpp - tt3::gui::CurrentActivity class implementation
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
#include "tt3-gui/API.hpp"
using namespace tt3::gui;

namespace tt3::gui
{
    extern CurrentSkin theCurrentSkin;
    extern CurrentCredentials theCurrentCredentials;
}

struct CurrentActivity::_Impl
{
    std::atomic<int>    instanceCount = 0;  //  ...to disallow a second instance
    tt3::util::Mutex    guard;
    tt3::ws::Activity   activity = nullptr;
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
void CurrentActivity::operator = (
        const tt3::ws::Activity & activity
    )
{
    try
    {
        replaceWith(activity);
    }
    catch (const tt3::util::Exception & ex)
    {
        qCritical() << ex;
    }
}

tt3::ws::Activity CurrentActivity::operator -> () const
{
    _Impl * impl = _impl();
    tt3::util::Lock _(impl->guard);
    Q_ASSERT(impl->instanceCount == 1);

    return impl->activity;
}

CurrentActivity::operator tt3::ws::Activity() const
{
    _Impl * impl = _impl();
    tt3::util::Lock _(impl->guard);
    Q_ASSERT(impl->instanceCount == 1);

    return impl->activity;
}

bool CurrentActivity::operator == (nullptr_t /*null*/) const
{
    _Impl * impl = _impl();
    tt3::util::Lock _(impl->guard);
    Q_ASSERT(impl->instanceCount == 1);

    return impl->activity.get() == nullptr;
}

bool CurrentActivity::operator != (nullptr_t /*null*/) const
{
    _Impl * impl = _impl();
    tt3::util::Lock _(impl->guard);
    Q_ASSERT(impl->instanceCount == 1);

    return impl->activity.get() != nullptr;
}

bool CurrentActivity::operator == (
        tt3::ws::Activity activity
    ) const
{
    _Impl * impl = _impl();
    tt3::util::Lock _(impl->guard);
    Q_ASSERT(impl->instanceCount == 1);

    return impl->activity == activity;
}

bool CurrentActivity::operator != (
        tt3::ws::Activity activity
    ) const
{
    _Impl * impl = _impl();
    tt3::util::Lock _(impl->guard);
    Q_ASSERT(impl->instanceCount == 1);

    return impl->activity != activity;
}

//////////
//  Operations
QDateTime CurrentActivity::lastChangedAt() const
{
    _Impl * impl = _impl();
    tt3::util::Lock _(impl->guard);
    Q_ASSERT(impl->instanceCount == 1);

    return impl->lastChangedAt;
}

bool CurrentActivity::replaceWith(
        tt3::ws::Activity with,
        const tt3::ws::Credentials & credentials
    )
{
    tt3::ws::Activity before, after;

    //  Change is effected in a "locked" state
    {
        _Impl * impl = _impl();
        tt3::util::Lock _(impl->guard);
        Q_ASSERT(impl->instanceCount == 1);

        if (with != impl->activity)
        {   //  Enter the comment if necessary
            QString comment;
            tt3::ws::Activities eventActivities;
            if (with != nullptr &&
                with->requireCommentOnStart(credentials))  //  may throw
            {
                EnterActivityStartCommentDialog dlg(theCurrentSkin->mainWindow(), with, credentials);   //  may throw
                if (dlg.doModal() != EnterActivityStartCommentDialog::Result::Ok)
                {   //  OOPS! The user has cancelled the change
                    return false;
                }
                comment = dlg.comment();
                eventActivities.insert(with);
                if (impl->activity != nullptr &&
                    impl->activity->requireCommentOnStop(credentials))   //  may throw
                {   //  Need to link event with BOTH activities!
                    eventActivities.insert(impl->activity);
                }
            }
            else if (impl->activity != nullptr &&
                     impl->activity->requireCommentOnStop(credentials))   //  may throw
            {
                EnterActivityStopCommentDialog dlg(theCurrentSkin->mainWindow(), impl->activity, credentials);  //  may throw
                if (dlg.doModal() != EnterActivityStopCommentDialog::Result::Ok)
                {   //  OOPS! The user has cancelled the change
                    return false;
                }
                comment = dlg.comment();
                eventActivities.insert(impl->activity);
            }
            //  Log current activity, if there is one, as a Work item
            QDateTime now = QDateTime::currentDateTimeUtc();
            tt3::ws::Account callerAccount =
                (with != nullptr) ?
                    with->workspace()->login(credentials) : //  may throw
                    (impl->activity != nullptr) ?
                        impl->activity->workspace()->login(credentials) :   //  may throw
                        nullptr;
            if (impl->activity != nullptr)
            {
                Q_ASSERT(callerAccount != nullptr);
                callerAccount->createWork(
                    credentials,
                    impl->lastChangedAt,
                    now,
                    impl->activity);
            }
            //  Log entered comment as an Event
            if (!comment.isEmpty())
            {
                Q_ASSERT(callerAccount != nullptr);
                callerAccount->createEvent( //  may throw
                    credentials, now, comment, eventActivities);
            }
            //  Make the change
            before = impl->activity;
            impl->activity = with;
            impl->lastChangedAt = now;
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

bool CurrentActivity::replaceWith(
        tt3::ws::Activity with
    )
{
    return replaceWith(with, theCurrentCredentials);
}

void CurrentActivity::drop()
{
    tt3::ws::Activity before, after;

    //  Change is effected in a "locked" state
    {
        _Impl * impl = _impl();
        tt3::util::Lock _(impl->guard);
        Q_ASSERT(impl->instanceCount == 1);

        if (impl->activity != nullptr)
        {   //  Make the change
            before = impl->activity;
            impl->activity = nullptr;
            impl->lastChangedAt = QDateTime::currentDateTimeUtc();
            after = impl->activity;
        }
    }
    //  Signal is sent in a "not locked" state
    if (before != after)
    {
        emit changed(before, after);
    }
}

//////////
//  Implementation helpers
CurrentActivity::_Impl * CurrentActivity::_impl()
{
    static _Impl impl;
    return &impl;
}

//////////
//  Global statics
namespace tt3::gui
{
    Q_DECL_EXPORT CurrentActivity theCurrentActivity;
}

//  End of tt3-gui/CurrentWorkspace.cpp
