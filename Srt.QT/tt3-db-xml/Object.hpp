//
//  tt3-db-xml/Object.hpp - a generic object in an XML file database
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
    /// \class Object tt3-db-xml/API.hpp
    /// \brief A generic object in an XML file database.
    class TT3_DB_XML_PUBLIC Object
        :   public virtual tt3::db::api::IObject
    {
        CANNOT_ASSIGN_OR_COPY_CONSTRUCT(Object)

        friend class Database;
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
        friend class WorkStream;

        //////////
        //  Construction/destruction (from DB type only)
    private:
        Object(Database * database, tt3::db::api::Oid oid);
        virtual ~Object();

        //////////
        //  tt3::db::api::IObject (general)
    public:
        virtual Database *  database() const override { return _database; }
        virtual tt3::db::api::Oid   oid() const override { return _oid; }
        virtual bool        isLive() const override;

        //////////
        //  tt3::db::api::IObject (reference counting)
    public:
        virtual State       state() const override;
        virtual int         referenceCount() const override;
        virtual void        addReference() override;
        virtual void        removeReference() override;

        //////////
        //  Implementation
    private:
        Database *const     _database;
        const tt3::db::api::Oid _oid;

        State           _state = State::New;
        int             _referenceCount = 0;
        bool            _isLive = true;

        //  Helpers
        void            _ensureLive() const;    //  throws tt3::db::api::DatabaseException
        void            _ensureLiveAndWritable() const;    //  throws tt3::db::api::DatabaseException
        virtual void    _markDead();

        //////////
        //  Serialization
    private:
        virtual void    _serializeProperties(
                                QDomElement & objectElement
                            ) const;
        virtual void    _serializeAggregations(
                                QDomElement & objectElement
                            ) const;
        virtual void    _serializeAssociations(
                                QDomElement & objectElement
                            ) const;

        virtual void    _deserializeProperties(
                                const QDomElement & objectElement
                            );  //  throws tt3::util::ParseException
        virtual void    _deserializeAggregations(
                                const QDomElement & objectElement
                            );  //  throws tt3::util::ParseException
        virtual void    _deserializeAssociations(
                                const QDomElement & objectElement
                            );  //  throws tt3::util::ParseException

        //////////
        //  Validation
    private:
        virtual void    _validate(
                                Objects & validatedObjects
                            );  //  throws tt3::db::api::DatabaseException
    };
}

//  End of tt3-db-xml/Object.hpp
