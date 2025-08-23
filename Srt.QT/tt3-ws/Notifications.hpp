//
//  tt3-ws/Notifications.hpp - workspace change notifications
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
#pragma once
#include "tt3-ws/API.hpp"

namespace tt3::ws
{
    //  The common base class for all workspace change notifications.
    class TT3_WS_PUBLIC ChangeNotification
    {
        //////////
        //  Construction/destruction/assignment
    public:
        ChangeNotification(const Workspace & workspace)
            :   _workspace(workspace) { Q_ASSERT(_workspace != nullptr); }
        virtual ~ChangeNotification() = default;
        //  Defult copy-constructor and assigmnent are OK

        //////////
        //  Operations
    public:
        //  The workspace where the change has occurred
        Workspace       workspace() const { return _workspace; }

        //////////
        //  Implementaton
    private:
        Workspace       _workspace; //  never nullptr
    };

    //  Emitted when a workspace is closed
    class TT3_WS_PUBLIC WorkspaceClosedNotification : public ChangeNotification
    {
        //////////
        //  Construction/destruction/assignment
    public:
        WorkspaceClosedNotification(const Workspace & workspace)
            :   ChangeNotification(workspace) {}
        //  Defult copy-constructor and assigmnent are OK
    };
}

Q_DECLARE_METATYPE(tt3::ws::ChangeNotification)
Q_DECLARE_METATYPE(tt3::ws::WorkspaceClosedNotification)

//  End of tt3-ws/Notifications.hpp
