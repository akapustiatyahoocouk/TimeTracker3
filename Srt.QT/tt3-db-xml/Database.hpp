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
        friend class Object;
        friend class Principal;
        friend class User;
        friend class Account;

        //////////
        //  Construction/destruction
    private:
        enum _Mode { _Create, _Open };
        Database(DatabaseAddress * address, _Mode mode) throws(DatabaseException);
    public:
        virtual ~Database();    //  closes database if still open

        //////////
        //  tt3::db::api::IDatabase (general)
    public:
        virtual DatabaseType *      type() const override { return DatabaseType::instance(); }
        virtual DatabaseAddress *   address() const override { return _address; }
        virtual tt3::db::api::IValidator *  validator() const override { return _validator; }
        virtual bool                isOpen() const override;
        virtual void                close() throws(DatabaseException) override;

        //////////
        //  tt3::db::api::IDatabase (associations)
    public:
        virtual tt3::db::api::Users     users() const throws(DatabaseException) override;
        virtual tt3::db::api::Accounts  accounts() const throws(DatabaseException) override;
        virtual tt3::db::api::IAccount *findAccount(const QString & login) const throws(DatabaseException) override;

        //////////
        //  tt3::db::api::IDatabase (access control)
    public:
        virtual tt3::db::api::IAccount *    tryLogin(const QString & login, const QString & password) const throws(DatabaseException) override;

        //////////
        //  tt3::db::api::IDatabase (life cycle)
    public:
        virtual tt3::db::api::IUser *   createUser(bool enabled, const QStringList & emailAddresses,
                                    const QString & realName,
                                    const std::optional<tt3::util::TimeSpan> & inactivityTimeout,
                                    const std::optional<QLocale> & uiLocale) throws(DatabaseException) override;

        //////////
        //  tt3::db::api::IDatabase (change notification handling)
    public:
        virtual tt3::db::api::ChangeNotifier &  changeNotifier() override { return _changeNotifier; }

        //////////
        //  Implementation
    private:
        DatabaseAddress *const  _address;
        tt3::db::api::IValidator *const _validator;

        mutable tt3::util::Mutex    _guard; //  for all access synchronization

        bool                _needsSaving = false;
        tt3::db::api::IObject::Oid  _nextUnusedOid = 1;

        //  Primary object caches - these contain all live
        //  objects, either directly (like Users) or indirectly
        //  (like Accounts, accessible through Users).
        QSet<User*>         _users; //  count as "references"

        //  Seconsary caches - these do NOT count as "references"
        QMap<tt3::db::api::IObject::Oid, Object*>
                            _liveObjects;   //  All "live" objects
        QSet<Object*>       _graveyard;     //  All "dead" objects

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

        tt3::db::api::ChangeNotifier    _changeNotifier;

        //  Helpers
        void            _ensureOpen() const throws (DatabaseException);
        void            _markClosed();
        Account *       _findAccount(const QString & login) const;

        //  Serialization
        void            _save() throws(DatabaseException);
        void            _load() throws(DatabaseException);
        QList<QDomElement>  _childElements(const QDomElement & parentElement, const QString & tagName);
        QDomElement     _childElement(const QDomElement & parentElement, const QString & tagName) throws(DatabaseException);
    };
}

//  End of tt3-db-api/Database.hpp
