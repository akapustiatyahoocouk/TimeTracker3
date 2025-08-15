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
    //  The common base class for all database change notifications.
    class TT3_DB_API_PUBLIC ChangeNotification
    {
        //////////
        //  Construction/destruction/assignment
    protected:
        ChangeNotification(IDatabase * db)
            :   database(db) { Q_ASSERT(database != nullptr); }
        virtual ~ChangeNotification() = default;

        //  Defult copy-constructor and assigmnent are OK

        //////////
        //  Properties
    public:
        //  The database where the change has occurred
        IDatabase *const    database;
    };

    //  Issued after a database is closed
    class TT3_DB_API_PUBLIC DatabaseClosedNotification : public ChangeNotification
    {
        //////////
        //  Construction/destruction/assignment
    public:
        DatabaseClosedNotification(IDatabase * db)
            :   ChangeNotification(db) {}

        //  Defult copy-constructor and assigmnent are OK
    };

    //  A per-database agent that emits change
    //  notification signals for that database.
    class TT3_DB_API_PUBLIC ChangeNotifier final : public QObject
    {
        Q_OBJECT
        CANNOT_ASSIGN_OR_COPY_CONSTRUCT(ChangeNotifier)

        //////////
        //  Construction/destruction
    public:
        ChangeNotifier();
        ~ChangeNotifier();

        //////////
        //  Operations
    public:
        //  Posts the spefified change notification to this
        //  notifier, taking ownership of that change notification.
        //  The posted change notification will be emitted as a
        //  corresponding signal from the notifier as soon as
        //  practicable.
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
        void        databaseClosed(DatabaseClosedNotification notification);

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

//  End of tt3-db-api/Notifications.hpp
