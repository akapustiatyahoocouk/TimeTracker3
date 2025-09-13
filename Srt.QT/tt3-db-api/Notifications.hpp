//
//  tt3-db-api/Notifications.hpp - database change notifications
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
#include "tt3-db-api/API.hpp"

namespace tt3::db::api
{
    /// \class ChangeNotification tt3-db-api/API.hpp
    /// \brief
    ///     The common base class for all database
    ///     change notifications.
    class TT3_DB_API_PUBLIC ChangeNotification
    {
        //////////
        //  Construction/destruction/assignment
    protected:
        /// \brief
        ///     Constructs the change notification.
        /// \param db
        ///     The database where the change has occurred.
        ChangeNotification(IDatabase * db)
            :   _database(db) { Q_ASSERT(_database != nullptr); }
    public:
        /// \brief
        ///     The dfault [empty] destructor.
        virtual ~ChangeNotification() = default;

        //  Default copy-constructor and assigmnent are OK

        //////////
        //  Properties
    public:
        /// \brief
        ///     Returns the database where the change has occurred
        /// \return
        ///     The database where the change has occurred
        IDatabase *     database() const { return _database; }

        //////////
        //  Implementation
    private:
        IDatabase *     _database;
    };

    /// \class DatabaseClosedNotification tt3-db-api/API.hpp
    /// \brief Issued after a database is closed.
    class TT3_DB_API_PUBLIC DatabaseClosedNotification : public ChangeNotification
    {
        //////////
        //  Construction/destruction/assignment
    public:
        /// \brief
        ///     Constructs the change notification.
        /// \param db
        ///     The database that has been closed.
        DatabaseClosedNotification(IDatabase * db)
            :   ChangeNotification(db) {}

        //  Default copy-constructor, destructor and
        //  assigmnent are all OK
    };

    /// \class ObjectCreatedNotification tt3-db-api/API.hpp
    /// \brief Issued after a new object is created in a database.
    class TT3_DB_API_PUBLIC ObjectCreatedNotification : public ChangeNotification
    {
        //////////
        //  Construction/destruction/assignment
    public:
        /// \brief
        ///     Constructs the change notification.
        /// \param db
        ///     The database where an object has been created.
        /// \param objectType
        ///     The type of the newly created object.
        /// \param oid
        ///     The OID of the newly created object.
        ObjectCreatedNotification(IDatabase * db, IObjectType * objectType, const Oid & oid)
            :   ChangeNotification(db), _objectType(objectType), _oid(oid)
        { Q_ASSERT(_objectType != nullptr && _oid != Oid::Invalid); }

        //  Default copy-constructor, destructor and
        //  assigmnent are all OK

        //////////
        //  Operations
    public:
        /// \brief
        ///     Returns the type of the newly created object.
        /// \return
        ///     The type of the newly created object.
        IObjectType *   objectType() const { return _objectType; }

        /// \brief
        ///     Returns the OID of the newly created object.
        /// \return
        ///     The OID of the newly created object.
        Oid             oid() const { return _oid; }

        //////////
        //  Implementation
    private:
        IObjectType *   _objectType;
        Oid             _oid;
    };

    /// \class ObjectDestroyedNotification tt3-db-api/API.hpp
    /// \brief Issued after an object is destroyed in a database.
    class TT3_DB_API_PUBLIC ObjectDestroyedNotification : public ChangeNotification
    {
        //////////
        //  Construction/destruction/assignment
    public:
        /// \brief
        ///     Constructs the change notification.
        /// \param db
        ///     The database where an object has been destroyed.
        /// \param objectType
        ///     The type of the destroyed object.
        /// \param oid
        ///     The OID of the destroyed object.
        ObjectDestroyedNotification(IDatabase * db, IObjectType * objectType, const Oid & oid)
            :   ChangeNotification(db), _objectType(objectType), _oid(oid)
        { Q_ASSERT(_objectType != nullptr && _oid != Oid::Invalid); }

        //  Default copy-constructor, destructor and
        //  assigmnent are all OK

        //////////
        //  Operations
    public:
        /// \brief
        ///     Returns the type of the destroyed object.
        /// \return
        ///     The type of the destroyed object.
        IObjectType *   objectType() const { return _objectType; }

        /// \brief
        ///     Returns the OID of the destroyed object.
        /// \return
        ///     The OID of the destroyed object.
        Oid             oid() const { return _oid; }

        //////////
        //  Implementation
    private:
        IObjectType *   _objectType;
        Oid             _oid;
    };

    /// \class ObjectModifiedNotification tt3-db-api/API.hpp
    /// \brief
    ///     Issued after an object is modified in a database.
    /// \details
    ///     This includes both modifying object's properties
    ///     and modifying its associations.
    class TT3_DB_API_PUBLIC ObjectModifiedNotification : public ChangeNotification
    {
        //////////
        //  Construction/destruction/assignment
    public:
        /// \brief
        ///     Constructs the change notification.
        /// \param db
        ///     The database where an object has been modified.
        /// \param objectType
        ///     The type of the modified object.
        /// \param oid
        ///     The OID of the modified object.
        ObjectModifiedNotification(IDatabase * db, IObjectType * objectType, const Oid & oid)
            :   ChangeNotification(db), _objectType(objectType), _oid(oid)
        { Q_ASSERT(_objectType != nullptr && _oid != Oid::Invalid); }

        //  Default copy-constructor, destructor and
        //  assigmnent are all OK

        //////////
        //  Operations
    public:
        /// \brief
        ///     Returns the type of the modified object.
        /// \return
        ///     The type of the modified object.
        IObjectType *   objectType() const { return _objectType; }

        /// \brief
        ///     Returns the OID of the modified object.
        /// \return
        ///     The OID of the modified object.
        Oid             oid() const { return _oid; }

        //////////
        //  Implementation
    private:
        IObjectType *   _objectType;
        Oid             _oid;
    };

    /// \class ChangeNotifier tt3-db-api/API.hpp
    /// \brief
    ///     A per-database agent that emits change
    ///     notification signals for that database.
    class TT3_DB_API_PUBLIC ChangeNotifier final : public QObject
    {
        Q_OBJECT
        CANNOT_ASSIGN_OR_COPY_CONSTRUCT(ChangeNotifier)

        //////////
        //  Construction/destruction
    public:
        /// \brief
        ///     The class constructor.
        /// \details
        ///     Starts the hidden notification dispatch thread.
        ChangeNotifier();

        /// \brief
        ///     The class denstructor.
        /// \details
        ///     Stops the hidden notification dispatch thread.
        ~ChangeNotifier();

        //////////
        //  Operations
    public:
        /// \brief
        ///     Posts the spefified change notification to this
        ///     notifier, taking ownership of that change notification.
        /// \details
        ///     The posted change notification will be emitted as a
        ///     corresponding signal from the notifier as soon as
        ///     practicable. All signals are raised on a hidden
        ///     notification dispatch thread, which automatically
        ///     means that connections of any slots to these signals
        ///     become queued connections.
        void            post(ChangeNotification * notification);

        //////////
        //  Signals
        //  All signals are emitted on a hidden worker
        //  thread private to a specific notifier. As a
        //  consequence, any e.g. UI thread connecting
        //  one of its slots to these notifier signals
        //  will automatically use "queued connections",
        //  thus making surthese slots are invoked on
        //  an UI thread.
    signals:
        /// \brief
        ///     Emitted after a database is closed
        /// \param notification
        ///     The details of the change notification.
        void        databaseClosed(DatabaseClosedNotification notification);

        /// \brief
        ///     Emitted after a new object is created
        /// \param notification
        ///     The details of the change notification.
        void        objectCreated(ObjectCreatedNotification notification);

        /// \brief
        ///     Emitted after an object is destroyed
        /// \param notification
        ///     The details of the change notification.
        void        objectDestroyed(ObjectDestroyedNotification notification);

        /// \brief
        ///     Emitted after an object is modified
        /// \param notification
        ///     The details of the change notification.
        void        objectModified(ObjectModifiedNotification notification);

        //////////
        //  Implementation
    private:
        //  post nullptr to the queue to stop the worker thread
        tt3::util::BlockingQueue<ChangeNotification*>   _pendingNotifications;

        //  The worker thread is where signals are emitted
        class TT3_DB_API_PUBLIC _WorkerThread : public QThread
        {
            CANNOT_ASSIGN_OR_COPY_CONSTRUCT(_WorkerThread)

            //////////
            //  Constants
        public:
            static inline int   WaitChunkMs = 250;

            //////////
            //  Construction/destruction
        public:
            explicit _WorkerThread(ChangeNotifier * changeNotifier)
                :   _changeNotifier(changeNotifier) {}
            virtual ~_WorkerThread() = default;

            //////////
            //  QThread
        protected:
            virtual void    run() override;

            //////////
            //  Implementation
        private:
            ChangeNotifier *const   _changeNotifier;
        };
        _WorkerThread   _workerThread;
    };
}

Q_DECLARE_METATYPE(tt3::db::api::ChangeNotification)
Q_DECLARE_METATYPE(tt3::db::api::DatabaseClosedNotification)
Q_DECLARE_METATYPE(tt3::db::api::ObjectCreatedNotification)
Q_DECLARE_METATYPE(tt3::db::api::ObjectDestroyedNotification)
Q_DECLARE_METATYPE(tt3::db::api::ObjectModifiedNotification)

//  End of tt3-db-api/Notifications.hpp
