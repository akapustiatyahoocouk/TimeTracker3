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
    class TT3_WS_PUBLIC WorkspaceClosedNotification
        :   public ChangeNotification
    {
        //////////
        //  Construction/destruction/assignment
    public:
        WorkspaceClosedNotification(const Workspace & workspace)
            :   ChangeNotification(workspace) {}
        //  Defult copy-constructor and assigmnent are OK
    };

    //  Emitted after a new object is created
    class TT3_WS_PUBLIC ObjectCreatedNotification
        :   public ChangeNotification
    {
        //////////
        //  Construction/destruction/assignment
    public:
        ObjectCreatedNotification(const Workspace & workspace, ObjectType * objectType, const Oid & oid)
            :   ChangeNotification(workspace), _objectType(objectType), _oid(oid)
            { Q_ASSERT(_objectType != nullptr && _oid != Oid::Invalid); }
        //  Defult copy-constructor and assigmnent are OK

        //////////
        //  Operations
    public:
        //  The type of the created object
        ObjectType *    objectType() const { return _objectType; }

        //  The OID of the created object
        Oid             oid() const { return _oid; }

        //////////
        //  Implementation
    private:
        ObjectType *    _objectType;
        Oid             _oid;
    };

    //  Emitted after an object is destroyed
    class TT3_WS_PUBLIC ObjectDestroyedNotification
        :   public ChangeNotification
    {
        //////////
        //  Construction/destruction/assignment
    public:
        ObjectDestroyedNotification(const Workspace & workspace, ObjectType * objectType, const Oid & oid)
            :   ChangeNotification(workspace), _objectType(objectType), _oid(oid)
            { Q_ASSERT(_objectType != nullptr && _oid != Oid::Invalid); }
        //  Defult copy-constructor and assigmnent are OK

        //////////
        //  Operations
    public:
        //  The type of the destroyed object
        ObjectType *    objectType() const { return _objectType; }

        //  The OID of the destroyed object
        Oid             oid() const { return _oid; }

        //////////
        //  Implementation
    private:
        ObjectType *    _objectType;
        Oid             _oid;
    };

    //  Emitted after an object is modified.
    //  This includes both changes to object's
    //  properties and changes to its associations.
    class TT3_WS_PUBLIC ObjectModifiedNotification
        :   public ChangeNotification
    {
        //////////
        //  Construction/destruction/assignment
    public:
        ObjectModifiedNotification(const Workspace & workspace, ObjectType * objectType, const Oid & oid)
            :   ChangeNotification(workspace), _objectType(objectType), _oid(oid)
            { Q_ASSERT(_objectType != nullptr && _oid != Oid::Invalid); }
        //  Defult copy-constructor and assigmnent are OK

        //////////
        //  Operations
    public:
        //  The type of the modified object
        ObjectType *    objectType() const { return _objectType; }

        //  The OID of the modified object
        Oid             oid() const { return _oid; }

        //////////
        //  Implementation
    private:
        ObjectType *    _objectType;
        Oid             _oid;
    };
}

Q_DECLARE_METATYPE(tt3::ws::ChangeNotification)
Q_DECLARE_METATYPE(tt3::ws::WorkspaceClosedNotification)
Q_DECLARE_METATYPE(tt3::ws::ObjectCreatedNotification)
Q_DECLARE_METATYPE(tt3::ws::ObjectDestroyedNotification)
Q_DECLARE_METATYPE(tt3::ws::ObjectModifiedNotification)

//  End of tt3-ws/Notifications.hpp
