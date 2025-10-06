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
    /// \class Database tt3-db-xml/API.hpp
    /// \brief A single-user database stored in an XML file.
    class TT3_DB_XML_PUBLIC Database final
        :   public virtual tt3::db::api::IDatabase
    {
        CANNOT_ASSIGN_OR_COPY_CONSTRUCT(Database)

        friend class DatabaseType;
        friend class Object;
        friend class Principal;
        friend class User;
        friend class Account;
        friend class ActivityType;
        friend class Activity;
        friend class PublicActivity;
        friend class PrivateActivity;
        friend class Task;
        friend class PublicTask;
        friend class PrivateTask;
        friend class Workload;
        friend class Project;

        //////////
        //  Construction/destruction
    private:
        enum _OpenMode
        {
            _Create,
            _OpenReadOnly,
            _OpenReadWrite,
            _Dead
        };
        Database(
                DatabaseAddress * address,
                _OpenMode openMode
            );  //  throws tt3::db::api::DatabaseException
    public:
        virtual ~Database();    //  closes database if still open

        //////////
        //  tt3::db::api::IDatabase (general)
    public:
        virtual auto    type(
                            ) const -> DatabaseType * override;
        virtual auto    address(
                            ) const -> DatabaseAddress * override;
        virtual auto    validator(
                            ) const -> tt3::db::api::IValidator * override;
        virtual bool    isOpen() const override;
        virtual bool    isReadOnly() const override;
        virtual void    close() override;
        virtual void    refresh() override;

        //////////
        //  tt3::db::api::IDatabase (associations)
    public:
        virtual auto    users(
                            ) const -> tt3::db::api::Users override;
        virtual auto    accounts(
                            ) const -> tt3::db::api::Accounts override;
        virtual auto    findAccount(
                                const QString & login
                            ) const -> tt3::db::api::IAccount * override;
        virtual auto    activityTypes(
                            ) const -> tt3::db::api::ActivityTypes override;
        virtual auto    findActivityType(
                                const QString & displayName
                            ) -> tt3::db::api::IActivityType * override;
        virtual auto    publicActivities(
                            ) const -> tt3::db::api::PublicActivities;
        virtual auto    findPublicActivity(
                                const QString & displayName
                            ) -> tt3::db::api::IPublicActivity * override;
        virtual auto    publicActivitiesAndTasks(
                            ) const -> tt3::db::api::PublicActivities;
        virtual auto    publicTasks(
                            ) const -> tt3::db::api::PublicTasks;
        virtual auto    rootPublicTasks(
                            ) const -> tt3::db::api::PublicTasks;
        virtual auto    projects(
                            ) const -> tt3::db::api::Projects;
        virtual auto    rootProjects(
                            ) const -> tt3::db::api::Projects;
        virtual auto    workStreams(
                            ) const -> tt3::db::api::WorkStreams;
        virtual auto    beneficiaries(
                            ) const -> tt3::db::api::Beneficiaries;

        //////////
        //  tt3::db::api::IDatabase (access control)
    public:
        virtual auto    tryLogin(
                                const QString & login,
                                const QString & password
                            ) const -> tt3::db::api::IAccount * override;
        virtual auto    login(
                                const QString & login,
                                const QString & password
                            ) const -> tt3::db::api::IAccount * override;

        //////////
        //  tt3::db::api::IDatabase (life cycle)
    public:
        virtual auto    createUser(
                                bool enabled,
                                const QStringList & emailAddresses,
                                const QString & realName,
                                const tt3::db::api::InactivityTimeout & inactivityTimeout,
                                const tt3::db::api::UiLocale & uiLocale
                            ) -> tt3::db::api::IUser * override;
        virtual auto    createActivityType(
                                const QString & displayName,
                                const QString & description
                            ) -> tt3::db::api::IActivityType * override;
        virtual auto    createPublicActivity(
                                const QString & displayName,
                                const QString & description,
                                const tt3::db::api::InactivityTimeout & timeout,
                                bool requireCommentOnStart,
                                bool requireCommentOnStop,
                                bool fullScreenReminder,
                                tt3::db::api::IActivityType * activityType,
                                tt3::db::api::IWorkload * workload
                            ) -> tt3::db::api::IPublicActivity * override;
        virtual auto    createPublicTask(
                                const QString & displayName,
                                const QString & description,
                                const tt3::db::api::InactivityTimeout & timeout,
                                bool requireCommentOnStart,
                                bool requireCommentOnStop,
                                bool fullScreenReminder,
                                tt3::db::api::IActivityType * activityType,
                                tt3::db::api::IWorkload * workload,
                                bool completed,
                                bool requireCommentOnCompletion
                            ) -> tt3::db::api::IPublicTask * override;
        virtual auto    createProject(
                                const QString & displayName,
                                const QString & description,
                                const tt3::db::api::Beneficiaries & beneficiaries,
                                bool completed
                            ) -> tt3::db::api::IProject * override;
        virtual auto    createWorkStream(
                                const QString & displayName,
                                const QString & description,
                                const tt3::db::api::Beneficiaries & beneficiaries
                            ) -> tt3::db::api::IWorkStream * override;
        virtual auto    createBeneficiary(
                                const QString & displayName,
                                const QString & description
                            ) -> tt3::db::api::IBeneficiary * override;

        //////////
        //  tt3::db::api::IDatabase (change notification handling)
    public:
        virtual auto    changeNotifier(
                            ) -> tt3::db::api::ChangeNotifier * override
        {
            return &_changeNotifier;
        }

        //////////
        //  Implementation
    private:
        DatabaseAddress *const          _address;   //  counts as a "reference"
        tt3::db::api::IValidator *const _validator;
        mutable tt3::util::Mutex        _guard; //  for all access synchronization
        bool            _needsSaving;
        bool            _isOpen;
        bool            _isReadOnly;    //  not "const" - will be faked as "false" during close()

        //  Primary object caches - these contain all live
        //  objects, either directly (like Users) or indirectly
        //  (like Accounts, accessible through Users).
        Users               _users;             //  count as "references"
        ActivityTypes       _activityTypes;     //  count as "references"
        PublicActivities    _publicActivities;  //  BUT NOT TASKS! count as "references"
        PublicTasks         _rootPublicTasks;   //  count as "references"
        Projects            _rootProjects;      //  count as "references"

        //  Seconsary caches - these do NOT count as "references"
        QMap<tt3::db::api::Oid, Object*> _liveObjects;  //  All "live" objects
        QMap<tt3::db::api::Oid, Object*> _graveyard;    //  All "dead" objects

        //  Database file locking mechanism
        class TT3_DB_XML_PUBLIC _LockRefresher final
            :   public QThread
        {
            CANNOT_ASSIGN_OR_COPY_CONSTRUCT(_LockRefresher)

            //////////
            //  Constants
        public:
            static const inline int RefreshIntervalMin = 1;
            static const inline int StaleTimeoutMin = 5;

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
        void                _ensureOpen() const;    //  throws tt3::db::api::DatabaseException
        void                _ensureOpenAndWritable() const; //  throws tt3::db::api::DatabaseException
        void                _markModified();
        void                _markClosed();
        tt3::db::api::Oid   _generateOid();
        Account *           _findAccount(const QString & login) const;
        ActivityType *      _findActivityType(const QString & displayName) const;
        PublicActivity *    _findPublicActivity(const QString & displayName) const;
        PublicTask *        _findRootPublicTask(const QString & displayName) const;
        Project *           _findRootProject(const QString & displayName) const;

        //  Serialization
        void            _save();    //  throws tt3::util::Exception
        template <class T>
        QList<T>        _sortedByOid(const QSet<T> & objects)
        {
            QList<T> result = objects.values();
            std::sort(result.begin(),
                      result.end(),
                      [](T a, T b) { return a->_oid < b->_oid; });
            return result;
        }
        template <class R, class T>
        QList<R>        _map(const QList<T> & objects,
                             std::function<R(T)> mapper)
        {
            QList<R> result;
            for (T obj : objects)
            {
                result.append(mapper(obj));
            }
            return result;
        }
        template <class T>
        void            _serializeAssociation(
                                QDomElement & objectElement,
                                const QString & associationName,
                                const T * association
                            )
        {
            if (association != nullptr)
            {
                objectElement.setAttribute(associationName, tt3::util::toString(association->_oid));
            }
        }
        template <class T>
        void            _serializeAssociation(
                                QDomElement & objectElement,
                                const QString & associationName,
                                const QList<T*> association
                            )
        {
            if (!association.isEmpty())
            {
                objectElement.setAttribute(
                    associationName,
                    _map<QString,T*>(
                        association,
                        [](auto a) { return tt3::util::toString(a->_oid); })
                        .join(","));
            }
        }
        template <class T>
        void            _serializeAssociation(
                                QDomElement & objectElement,
                                const QString & associationName,
                                const QSet<T*> association
                            )
        {
            _serializeAssociation(objectElement, associationName, _sortedByOid(association));
        }
        template <class T>
        void            _serializeObject(
                                QDomElement & collectionElement,
                                const T * object
                            )
        {
            QDomElement objectElement =
                collectionElement.ownerDocument()
                                 .createElement(object->type()->mnemonic().toString());
            collectionElement.appendChild(objectElement);
            //  Serialize features
            object->_serializeProperties(objectElement);
            object->_serializeAggregations(objectElement);
            object->_serializeAssociations(objectElement);
        }
        template <class T>
        void            _serializeAggregation(
                                QDomElement & parentElement,
                                const QString & aggregationName,
                                const QSet<T*> & aggregation
                            )
        {
            QDomElement aggregationElement =
                parentElement.ownerDocument().createElement(aggregationName);
            parentElement.appendChild(aggregationElement);
            for (T * object : _sortedByOid(aggregation))
            {   //  Sorting by OID to reduce changes
                _serializeObject(aggregationElement, object);
            }
        }

        //  Deserialization
        QMap<Object*, QDomElement>  _deserializationMap;    //  object -> DOM element from which it came

        void            _load();    //  throws tt3::util::Exception
        auto            _childElements(
                                const QDomElement & parentElement,
                                const QString & tagName
                            ) -> QList<QDomElement>;
        auto            _childElement(  //  throws tt3::db::api::DatabaseException
                                const QDomElement & parentElement,
                                const QString & tagName
                            ) -> QDomElement;
        template <class T>
        T               _getObject(const tt3::db::api::Oid oid)
        {
            if (_liveObjects.contains(oid))
            {
                if (T t = dynamic_cast<T>(_liveObjects[oid]))
                {
                    return t;
                }
            }
            throw tt3::db::api::DatabaseCorruptException(_address);
        }
        template <class T>
        void            _deserializeAssociation(
                                const QDomElement & objectElement,
                                const QString & associationName,
                                T *& association
                            )
        {
            Q_ASSERT(association == nullptr);
            if (objectElement.hasAttribute(associationName))
            {
                association =
                    _getObject<T*>(
                        tt3::util::fromString<tt3::db::api::Oid>(
                            objectElement.attribute(associationName)));
                association->addReference();
            }
        }
        template <class T>
        void            _deserializeAssociation(
                                const QDomElement & objectElement,
                                const QString & associationName,
                                QList<T*> & association
                            )
        {
            Q_ASSERT(association.isEmpty());
            if (objectElement.hasAttribute(associationName))
            {
                association =
                    _map<T*,QString>(
                        objectElement.attribute(associationName).split(','),
                        [&](auto s)
                        {
                            return _getObject<T*>(
                                tt3::util::fromString(s, tt3::db::api::Oid::Invalid));
                        });
                for (T * a : association)
                {
                    a->addReference();
                }
            }
        }
        template <class T>
        void            _deserializeAssociation(
                                const QDomElement & objectElement,
                                const QString & associationName,
                                QSet<T*> & association
                            )
        {
            Q_ASSERT(association.isEmpty());
            QList<T*> temp;
            _deserializeAssociation(objectElement, associationName, temp);
            association = QSet<T*>(temp.cbegin(), temp.cend());
            if (association.size() != temp.size())
            {
                throw tt3::db::api::DatabaseCorruptException(_address);
            }
        }
        template <class T>
        void            _deserializeObject(
                                const QDomElement & objectElement,
                                T *& object,
                                std::function<T*(const tt3::db::api::Oid & oid)> objectFactory
                            )
        {
            tt3::db::api::Oid oid =
                tt3::util::fromString<tt3::db::api::Oid>(objectElement.attribute("OID", ""));
            if (!oid.isValid() || _liveObjects.contains(oid))
            {   //  OOPS!
                throw tt3::db::api::DatabaseCorruptException(_address);
            }
            object = objectFactory(oid);
            if (object == nullptr || !object->_isLive ||
                object->_database != this)
            {   //  OOPS!
                throw tt3::db::api::DatabaseCorruptException(_address);
            }
            object->_deserializeProperties(objectElement);
            object->_deserializeAggregations(objectElement);
        }
        template <class T>
        void            _deserializeAggregation(
                                const QDomElement & parentElement,
                                const QString & aggregationName,
                                QSet<T*> & aggregation,
                                std::function<T*(const tt3::db::api::Oid & oid)> objectFactory
            )
        {
            Q_ASSERT(aggregation.isEmpty());
            QDomElement aggregationElement = _childElement(parentElement, aggregationName); //  may throw
            tt3::db::api::IObjectType * objectType = _objectTypeTraits<T>();
            for (QDomElement objectElement : _childElements(aggregationElement, objectType->mnemonic().toString()))
            {
                T * object = nullptr;
                _deserializeObject<T>(
                    objectElement,
                    object,
                    objectFactory);
               aggregation.insert(object);
            }
        }

        template <class T>
        auto            _objectTypeTraits(
                            ) -> tt3::db::api::IObjectType *  = delete;
#define DECLARE_OBJECT_TYPE_TRAITS(TYPE)                               \
        template <>                                             \
        auto            _objectTypeTraits<TYPE>(                \
                            ) -> tt3::db::api::IObjectType *    \
        {                                                       \
            return tt3::db::api::ObjectTypes::TYPE::instance(); \
        }
        DECLARE_OBJECT_TYPE_TRAITS(User)
        DECLARE_OBJECT_TYPE_TRAITS(Account)
        DECLARE_OBJECT_TYPE_TRAITS(ActivityType)
        DECLARE_OBJECT_TYPE_TRAITS(PublicActivity)
        DECLARE_OBJECT_TYPE_TRAITS(PublicTask)
        DECLARE_OBJECT_TYPE_TRAITS(PrivateActivity)
        DECLARE_OBJECT_TYPE_TRAITS(PrivateTask)
        DECLARE_OBJECT_TYPE_TRAITS(Project)
        DECLARE_OBJECT_TYPE_TRAITS(WorkStream)
        DECLARE_OBJECT_TYPE_TRAITS(Beneficiary)
#undef DECLARE_OBJECT_TYPE_TRAITS

        //  Validation
        void            _validate();    //  throwstt3::db::api::DatabaseException
    };
}

//  End of tt3-db-xml/Database.hpp
