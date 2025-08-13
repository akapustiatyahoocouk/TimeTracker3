//
//  tt3-db-xml/Database.hpp - "XML file database"
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

namespace tt3::db::xml
{
    //////////
    //  A single-user database stored in an XML file
    class TT3_DB_XML_PUBLIC Database final
        :   public virtual tt3::db::api::IDatabase
    {
        CANNOT_ASSIGN_OR_COPY_CONSTRUCT(Database)

        friend class DatabaseType;

        //////////
        //  Construction/destruction
    private:
        enum _Mode { _Create, _Open };
        Database(DatabaseAddress * address, _Mode mode);
    public:
        virtual ~Database();    //  closes database if still open

        //////////
        //  tt3::db::api::IDatabase (general)
    public:
        virtual DatabaseType *     type() const override { return DatabaseType::instance(); }
        virtual DatabaseAddress *  address() const override { return _address; }
        virtual tt3::db::api::IValidator *  validator() const override { return _validator; }
        virtual bool                isOpen() const override;
        virtual void                close() throws(DatabaseException) override;

        //////////
        //  tt3::db::api::IDatabase (associations)
    public:
        virtual tt3::db::api::Users     users() const throws(DatabaseException) override;
        virtual tt3::db::api::Accounts  accounts() const throws(DatabaseException) override;

        //////////
        //  tt3::db::api::IDatabase (access control)
    public:
        virtual tt3::db::api::IAccount *    tryLogin(const QString & login, const QString & password) const throws(DatabaseException) override;

        //////////
        //  Implementation
    private:
        DatabaseAddress *const  _address;
        tt3::db::api::IValidator *const _validator;

        mutable tt3::util::Mutex    _guard; //  for all access synchronization

        bool                _needsSaving = false;
        tt3::db::api::IDatabaseObject::Oid  _nextUnusedOid = 1;

        //  Primary object caches (usable objects only)
        QSet<User*>         _users; //  count as "references"

        //  Delete-able instances: live == false && refcount == 0
        QSet<DatabaseObject*>   _graveyard; //  do NOT count as references

        //  Database file locking mechanism
        class TT3_DB_XML_PUBLIC _LockRefresher final : public QThread
        {
            CANNOT_ASSIGN_OR_COPY_CONSTRUCT(_LockRefresher)

            //////////
            //  Constants
        public:
            static inline int   RefreshIntervalMin = 1;
            static inline int   StaleTimeoutMin = 5;

            //////////
            //  Construction/destruction
        public:
            explicit _LockRefresher(Database * database);
            virtual ~_LockRefresher();

            //////////
            //  QThread
        protected:
            virtual void    run() override;

            //////////
            //  Operations
        public:
            void            requestStop() { _stopRequested = true; }

            //////////
            //  Implementation
        private:
            Database *const     _database;
            std::atomic<bool>   _stopRequested = false;
            QFile               _lockFile;
        };
        _LockRefresher *    _lockRefresher = nullptr;   //  nullptr == database is closed
    };
}

//  End of tt3-db-api/Database.hpp
