//
//  tt3-ws/CurrentWorkspace.cpp - tt3::gui::CurrentWorkspace class implementation
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

int CurrentWorkspace::_instanceCount = 0;
WorkspacePtr CurrentWorkspace::_currentWorkspace = nullptr;

//////////
//  Construction/destruction
CurrentWorkspace::CurrentWorkspace()
{
    Q_ASSERT(_instanceCount == 0);
    _instanceCount++;
}

CurrentWorkspace::~CurrentWorkspace()
{
    Q_ASSERT(_instanceCount == 1);
    _instanceCount--;
}

//////////
//  Operators
void CurrentWorkspace::operator = (const WorkspacePtr & workspace)
{
    Q_ASSERT(_instanceCount == 1);
    _currentWorkspace = workspace;
}

Workspace * CurrentWorkspace::operator -> () const
{
    Q_ASSERT(_instanceCount == 1);
    return _currentWorkspace.get();
}

CurrentWorkspace::operator WorkspacePtr() const
{
    Q_ASSERT(_instanceCount == 1);
    return _currentWorkspace;
}

bool CurrentWorkspace::operator == (nullptr_t null) const
{
    Q_ASSERT(_instanceCount == 1);
    return _currentWorkspace.get() == null;
}

bool CurrentWorkspace::operator != (nullptr_t null) const
{
    Q_ASSERT(_instanceCount == 1);
    return _currentWorkspace.get() != null;
}

//////////
//  Operations
Workspace * CurrentWorkspace::get() const
{
    Q_ASSERT(_instanceCount == 1);
    return _currentWorkspace.get();
}

void CurrentWorkspace::swap(WorkspacePtr & other)
{
    std::swap(_currentWorkspace, other);
}

//////////
//  Global statics
namespace tt3::ws
{
    Q_DECL_EXPORT CurrentWorkspace theCurrentWorkspace;
}

//  End of tt3-ws/CurrentWorkspace.cpp
