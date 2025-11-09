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
QString ActivityImpl::displayName(
        const Credentials & credentials
    ) const
{
    tt3::util::Lock _(_workspace->_guard);
    _ensureLive();  //  may throw

    try
    {
        //  Validate access rights
        if (!_canRead(credentials))
        {
            throw AccessDeniedException();
        }

        //  Do the work
        return _dataActivity->displayName();   //  may throw
    }
    catch (const tt3::util::Exception & ex)
    {   //  OOPS! Translate & re-throw
        WorkspaceException::translateAndThrow(ex);
    }
}

void ActivityImpl::setDisplayName(
        const Credentials & credentials,
        const QString & displayName
    )
{
    tt3::util::Lock _(_workspace->_guard);
    _ensureLive();  //  may throw

    try
    {
        //  Validate access rights
        if (!_canModify(credentials))
        {
            throw AccessDeniedException();
        }

        //  Do the work
        _dataActivity->setDisplayName(displayName); //  may throw
    }
    catch (const tt3::util::Exception & ex)
    {   //  OOPS! Translate & re-throw
        WorkspaceException::translateAndThrow(ex);
    }
}

QString ActivityImpl::description(
        const Credentials & credentials
    ) const
{
    tt3::util::Lock _(_workspace->_guard);
    _ensureLive();  //  may throw

    try
    {
        //  Validate access rights
        if (!_canRead(credentials))
        {
            throw AccessDeniedException();
        }

        //  Do the work
        return _dataActivity->description();    //  may throw
    }
    catch (const tt3::util::Exception & ex)
    {   //  OOPS! Translate & re-throw
        WorkspaceException::translateAndThrow(ex);
    }
}

void ActivityImpl::setDescription(
        const Credentials & credentials,
        const QString & description
    )
{
    tt3::util::Lock _(_workspace->_guard);
    _ensureLive();  //  may throw

    try
    {
        //  Validate access rights
        if (!_canModify(credentials))
        {
            throw AccessDeniedException();
        }

        //  Do the work
        _dataActivity->setDescription(description); //  may throw
    }
    catch (const tt3::util::Exception & ex)
    {   //  OOPS! Translate & re-throw
        WorkspaceException::translateAndThrow(ex);
    }
}

auto ActivityImpl::timeout(
        const Credentials & credentials
    ) const -> InactivityTimeout
{
    tt3::util::Lock _(_workspace->_guard);
    _ensureLive();  //  may throw

    try
    {
        //  Validate access rights
        if (!_canRead(credentials))
        {
            throw AccessDeniedException();
        }

        //  Do the work
        return _dataActivity->timeout();   //  may throw
    }
    catch (const tt3::util::Exception & ex)
    {   //  OOPS! Translate & re-throw
        WorkspaceException::translateAndThrow(ex);
    }
}

void ActivityImpl::setTimeout(
        const Credentials & credentials,
        const InactivityTimeout & timeout
    )
{
    tt3::util::Lock _(_workspace->_guard);
    _ensureLive();  //  may throw

    try
    {
        //  Validate access rights
        if (!_canModify(credentials))
        {
            throw AccessDeniedException();
        }

        //  Do the work
        _dataActivity->setTimeout(timeout); //  may throw
    }
    catch (const tt3::util::Exception & ex)
    {   //  OOPS! Translate & re-throw
        WorkspaceException::translateAndThrow(ex);
    }
}

bool ActivityImpl::requireCommentOnStart(
        const Credentials & credentials
    ) const
{
    tt3::util::Lock _(_workspace->_guard);
    _ensureLive();  //  may throw

    try
    {
        //  Validate access rights
        if (!_canRead(credentials))
        {
            throw AccessDeniedException();
        }

        //  Do the work
        return _dataActivity->requireCommentOnStart();   //  may throw
    }
    catch (const tt3::util::Exception & ex)
    {   //  OOPS! Translate & re-throw
        WorkspaceException::translateAndThrow(ex);
    }
}

void ActivityImpl::setRequireCommentOnStart(
        const Credentials & credentials,
        bool requireCommentOnStart
    )
{
    tt3::util::Lock _(_workspace->_guard);
    _ensureLive();  //  may throw

    try
    {
        //  Validate access rights
        if (!_canModify(credentials))
        {
            throw AccessDeniedException();
        }

        //  Do the work
        _dataActivity->setRequireCommentOnStart(requireCommentOnStart); //  may throw
    }
    catch (const tt3::util::Exception & ex)
    {   //  OOPS! Translate & re-throw
        WorkspaceException::translateAndThrow(ex);
    }
}

bool ActivityImpl::requireCommentOnStop(
        const Credentials & credentials
    ) const
{
    tt3::util::Lock _(_workspace->_guard);
    _ensureLive();  //  may throw

    try
    {
        //  Validate access rights
        if (!_canRead(credentials))
        {
            throw AccessDeniedException();
        }

        //  Do the work
        return _dataActivity->requireCommentOnStop();   //  may throw
    }
    catch (const tt3::util::Exception & ex)
    {   //  OOPS! Translate & re-throw
        WorkspaceException::translateAndThrow(ex);
    }
}

void ActivityImpl::setRequireCommentOnStop(
        const Credentials & credentials,
        bool requireCommentOnStop
    )
{
    tt3::util::Lock _(_workspace->_guard);
    _ensureLive();  //  may throw

    try
    {
        //  Validate access rights
        if (!_canModify(credentials))
        {
            throw AccessDeniedException();
        }

        //  Do the work
        _dataActivity->setRequireCommentOnStop(requireCommentOnStop); //  may throw
    }
    catch (const tt3::util::Exception & ex)
    {   //  OOPS! Translate & re-throw
        WorkspaceException::translateAndThrow(ex);
    }
}

bool ActivityImpl::fullScreenReminder(
        const Credentials & credentials
    ) const
{
    tt3::util::Lock _(_workspace->_guard);
    _ensureLive();  //  may throw

    try
    {
        //  Validate access rights
        if (!_canRead(credentials))
        {
            throw AccessDeniedException();
        }

        //  Do the work
        return _dataActivity->fullScreenReminder();   //  may throw
    }
    catch (const tt3::util::Exception & ex)
    {   //  OOPS! Translate & re-throw
        WorkspaceException::translateAndThrow(ex);
    }
}

void ActivityImpl::setFullScreenReminder(
        const Credentials & credentials,
        bool fullScreenReminder
    )
{
    tt3::util::Lock _(_workspace->_guard);
    _ensureLive();  //  may throw

    try
    {
        //  Validate access rights
        if (!_canModify(credentials))
        {
            throw AccessDeniedException();
        }

        //  Do the work
        _dataActivity->setFullScreenReminder(fullScreenReminder);   //  may throw
    }
    catch (const tt3::util::Exception & ex)
    {   //  OOPS! Translate & re-throw
        WorkspaceException::translateAndThrow(ex);
    }
}

//////////
//  Operations (associations)
auto ActivityImpl::activityType(
        const Credentials & credentials
    ) const -> ActivityType
{
    tt3::util::Lock _(_workspace->_guard);
    _ensureLive();  //  may throw

    try
    {
        //  Validate access rights
        if (!_canRead(credentials))
        {
            throw AccessDeniedException();
        }

        //  Do the work
        if (auto dataActivityType = _dataActivity->activityType())  //  may throw
        {
            return _workspace->_getProxy(dataActivityType);
        }
        return ActivityType();
    }
    catch (const tt3::util::Exception & ex)
    {   //  OOPS! Translate & re-throw
        WorkspaceException::translateAndThrow(ex);
    }
}

void ActivityImpl::setActivityType(
        const Credentials & credentials,
        ActivityType activityType
    )
{
    tt3::util::Lock _(_workspace->_guard);
    _ensureLive();  //  may throw
    if (activityType != nullptr)
    {   //  If in different Workspace, then in different
        //  Database, so don't need to check at this level
        activityType->_ensureLive();    //  may throw
    }

    try
    {
        //  Validate access rights
        if (!_canModify(credentials))
        {
            throw AccessDeniedException();
        }

        //  Do the work
        _dataActivity->setActivityType(
            (activityType != nullptr) ?
                activityType->_dataActivityType :
                nullptr);   //  may throw
    }
    catch (const tt3::util::Exception & ex)
    {   //  OOPS! Translate & re-throw
        WorkspaceException::translateAndThrow(ex);
    }
}

auto ActivityImpl::workload(
        const Credentials & credentials
    ) const -> Workload
{
    tt3::util::Lock _(_workspace->_guard);
    _ensureLive();  //  may throw

    try
    {
        //  Validate access rights
        if (!_canRead(credentials))
        {
            throw AccessDeniedException();
        }

        //  Do the work
        if (auto dataWorkload = _dataActivity->workload())  //  may throw
        {
            return _workspace->_getProxy(dataWorkload);
        }
        return Workload();
    }
    catch (const tt3::util::Exception & ex)
    {   //  OOPS! Translate & re-throw
        WorkspaceException::translateAndThrow(ex);
    }
}

void ActivityImpl::setWorkload(
        const Credentials & credentials,
        Workload workload
    )
{
    tt3::util::Lock _(_workspace->_guard);
    _ensureLive();  //  may throw
    if (workload != nullptr)
    {   //  If in different Workspace, then in different
        //  Database, so don't need to check at this level
        workload->_ensureLive();    //  may throw
    }

    try
    {
        //  Validate access rights
        if (!_canModify(credentials))
        {
            throw AccessDeniedException();
        }
        if (workload != nullptr &&
           !workload->_canModify(credentials))
        {
            throw AccessDeniedException();
        }

        //  Do the work
        _dataActivity->setWorkload(
            (workload != nullptr) ?
                workload->_dataWorkload :
                nullptr);   //  may throw
    }
    catch (const tt3::util::Exception & ex)
    {   //  OOPS! Translate & re-throw
        WorkspaceException::translateAndThrow(ex);
    }
}

auto ActivityImpl::works(
        const Credentials & credentials
    ) const -> Works
{
    tt3::util::Lock _(_workspace->_guard);
    _ensureLive();  //  may throw

    try
    {
        //  Validate access rights
        if (!_canRead(credentials))
        {
            throw AccessDeniedException();
        }

        //  Do the work
        Works result;
        for (auto dataWork : _dataActivity->works())    //  may throw
        {
            Work work = _workspace->_getProxy(dataWork);
            if (work->_canRead(credentials))    //  may throw
            {
                result.insert(work);
            }
        }
        return result;
    }
    catch (const tt3::util::Exception & ex)
    {   //  OOPS! Translate & re-throw
        WorkspaceException::translateAndThrow(ex);
    }
}

auto ActivityImpl::events(
        const Credentials & credentials
    ) const -> Events
{
    tt3::util::Lock _(_workspace->_guard);
    _ensureLive();  //  may throw

    try
    {
        //  Validate access rights
        if (!_canRead(credentials))
        {
            throw AccessDeniedException();
        }

        //  Do the work
        Events result;
        for (auto dataEvent : _dataActivity->events())  //  may throw
        {
            Event event = _workspace->_getProxy(dataEvent);
            if (event->_canRead(credentials))    //  may throw
            {
                result.insert(event);
            }
        }
        return result;
    }
    catch (const tt3::util::Exception & ex)
    {   //  OOPS! Translate & re-throw
        WorkspaceException::translateAndThrow(ex);
    }
}

//////////
//  Operations (access control)
bool ActivityImpl::canStart(
        const Credentials & credentials
    ) const
{
    tt3::util::Lock _(_workspace->_guard);
    _ensureLive();  //  may throw

    try
    {
        //  Validate access rights
        if (_workspace->_isBackupCredentials(credentials) ||
            _workspace->_isReportCredentials(credentials))
        {   //  Special access - cannot modify anything
            return false;
        }
        if (!_workspace->_isRestoreCredentials(credentials))
        {   //  No special access - use standard access rules
            Capabilities capabilities =
                _workspace->_validateAccessRights(credentials);
            if (!capabilities.contains(Capability::Administrator) &&
                !capabilities.contains(Capability::LogWork))
            {   //  OOPS! The caller won't be able to record the Work unit
                return false;
            }
            if (_dataActivity->requireCommentOnStart() ||
                _dataActivity->requireCommentOnStop())
            {   //  Will need to log an Event before/after a Work item...
                if (!capabilities.contains(Capability::Administrator) &&
                    !capabilities.contains(Capability::LogEvents))
                {   //  ...but won't be able to!
                    return false;
                }
            }
            if (auto dataTask =
                dynamic_cast<tt3::db::api::ITask*>(_dataActivity))
            {
                if (dataTask->completed())
                {
                    return false;
                }
            }
        }   //  Else special access - can modify anything
        return true;
    }
    catch (const tt3::util::Exception & ex)
    {   //  OOPS! Translate & re-throw
        WorkspaceException::translateAndThrow(ex);
    }
}

bool ActivityImpl::canStop(
        const Credentials & credentials
    ) const
{
    tt3::util::Lock _(_workspace->_guard);
    _ensureLive();  //  may throw

    try
    {
        //  Validate access rights
        if (_workspace->_isBackupCredentials(credentials) ||
            _workspace->_isReportCredentials(credentials))
        {   //  Special access - cannot modify anything
            return false;
        }
        if (!_workspace->_isRestoreCredentials(credentials))
        {   //  No special access - use standard access rules
            Capabilities capabilities =
                _workspace->_validateAccessRights(credentials);
            if (!capabilities.contains(Capability::Administrator) &&
                !capabilities.contains(Capability::LogWork))
            {   //  OOPS! The caller won't be able to record the Work unit
                return false;
            }
            if (_dataActivity->requireCommentOnStop())
            {   //  Will need to log an Event after a Work item...
                if (!capabilities.contains(Capability::Administrator) &&
                    !capabilities.contains(Capability::LogEvents))
                {   //  ...but won't be able to!
                    return false;
                }
            }
        }   //  Else special access - can modify anything
        return true;
    }
    catch (const tt3::util::Exception & ex)
    {   //  OOPS! Translate & re-throw
        WorkspaceException::translateAndThrow(ex);
    }
}

//  End of tt3-ws/ActivityImpl.cpp
