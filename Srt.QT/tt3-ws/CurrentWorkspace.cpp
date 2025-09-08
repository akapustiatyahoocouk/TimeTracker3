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

struct CurrentWorkspace::_Impl
{
    std::atomic<int>    instanceCount = 0;  //  ...to disallow a second instance
    tt3::util::Mutex    guard;
    Workspace           workspace = nullptr;
};

//////////
//  Construction/destruction
CurrentWorkspace::CurrentWorkspace()
{
    _Impl * impl = _impl();
    Q_ASSERT(impl->instanceCount == 0);
    impl->instanceCount++;
}

CurrentWorkspace::~CurrentWorkspace()
{
    _Impl * impl = _impl();
    Q_ASSERT(impl->instanceCount == 1);
    impl->instanceCount--;
}

//////////
//  Operators
void CurrentWorkspace::operator = (const Workspace & workspace)
{
    Workspace before, after;

    //  Change is effected in a "locked" state
    {
        _Impl * impl = _impl();
        tt3::util::Lock lock(impl->guard);

        Q_ASSERT(impl->instanceCount == 1);
        if (workspace != impl->workspace)
        {
            before = impl->workspace;
            impl->workspace = workspace;
            after = impl->workspace;
        }
    }
    //  Signal is sent in a "not locked" state
    if (before != after)
    {
        emit changed(before, after);
    }
}

Workspace CurrentWorkspace::operator -> () const
{
    _Impl * impl = _impl();
    tt3::util::Lock lock(impl->guard);

    Q_ASSERT(impl->instanceCount == 1);
    return impl->workspace;
}

CurrentWorkspace::operator Workspace() const
{
    _Impl * impl = _impl();
    tt3::util::Lock lock(impl->guard);

    Q_ASSERT(impl->instanceCount == 1);
    return impl->workspace;
}

bool CurrentWorkspace::operator == (nullptr_t /*null*/) const
{
    _Impl * impl = _impl();
    tt3::util::Lock lock(impl->guard);

    Q_ASSERT(impl->instanceCount == 1);
    return impl->workspace.get() == nullptr;
}

bool CurrentWorkspace::operator != (nullptr_t /*null*/) const
{
    _Impl * impl = _impl();
    tt3::util::Lock lock(impl->guard);

    Q_ASSERT(impl->instanceCount == 1);
    return impl->workspace.get() != nullptr;
}

//////////
//  Operations
void CurrentWorkspace::swap(Workspace & other)
{
    Workspace before, after;

    //  Change is effected in a "locked" state
    {
        _Impl * impl = _impl();
        tt3::util::Lock lock(impl->guard);

        Q_ASSERT(impl->instanceCount == 1);
        if (other != impl->workspace)
        {
            before = impl->workspace;
            std::swap(impl->workspace, other);
            after = impl->workspace;
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
auto CurrentWorkspace::_impl()
    -> CurrentWorkspace::_Impl *
{
    static _Impl impl;
    return &impl;
}

//////////
//  Global statics
namespace tt3::ws
{
    Q_DECL_EXPORT CurrentWorkspace theCurrentWorkspace;
}

//  End of tt3-ws/CurrentWorkspace.cpp
