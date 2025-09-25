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
        const Credentials & /*credentials*/
    ) const
{
    throw tt3::util::NotImplementedError();
}

void TaskImpl::setCompleted(
        const Credentials & /*credentials*/,
        bool /*completed*/
    )
{
    throw tt3::util::NotImplementedError();
}

bool TaskImpl::requireCommentOnCompletion(
        const Credentials & /*credentials*/
    ) const
{
    throw tt3::util::NotImplementedError();
}

void TaskImpl::setRequireCommentOnCompletion(
        const Credentials & /*credentials*/,
        bool /*requireCommentOnCompletion*/
    )
{
    throw tt3::util::NotImplementedError();
}

//  End of tt3-ws/TaskImpl.cpp
