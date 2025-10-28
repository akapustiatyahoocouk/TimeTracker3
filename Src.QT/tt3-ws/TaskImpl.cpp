//
//  tt3-ws/TaskImpl.cpp - tt3::ws::TaskImpl class implementation
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
TaskImpl::TaskImpl(
        Workspace workspace,
        tt3::db::api::ITask * dataTask
    ) : ActivityImpl(workspace, dataTask),
        _dataTask(dataTask)
{
    _dataTask->addReference();
}

TaskImpl::~TaskImpl()
{
    _dataTask->removeReference();
}

//////////
//  Operations (properties)
bool TaskImpl::completed(
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
        return _dataTask->completed();  //  may throw
    }
    catch (const tt3::util::Exception & ex)
    {   //  OOPS! Translate & re-throw
        WorkspaceException::translateAndThrow(ex);
    }
}

void TaskImpl::setCompleted(
        const Credentials & credentials,
        bool completed
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
        _dataTask->setCompleted(completed); //  may throw
    }
    catch (const tt3::util::Exception & ex)
    {   //  OOPS! Translate & re-throw
        WorkspaceException::translateAndThrow(ex);
    }
}

bool TaskImpl::requireCommentOnCompletion(
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
        return _dataTask->requireCommentOnCompletion(); //  may throw
    }
    catch (const tt3::util::Exception & ex)
    {   //  OOPS! Translate & re-throw
        WorkspaceException::translateAndThrow(ex);
    }
}

void TaskImpl::setRequireCommentOnCompletion(
        const Credentials & credentials,
        bool requireCommentOnCompletion
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
        _dataTask->setRequireCommentOnCompletion(requireCommentOnCompletion);   //  may throw
    }
    catch (const tt3::util::Exception & ex)
    {   //  OOPS! Translate & re-throw
        WorkspaceException::translateAndThrow(ex);
    }
}

//  End of tt3-ws/TaskImpl.cpp
