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
    //////////
    //  A generic object in an XML file database
    class TT3_DB_XML_PUBLIC Object
        :   public virtual tt3::db::api::IObject
    {
        CANNOT_ASSIGN_OR_COPY_CONSTRUCT(Object)

        friend class Database;
        friend class Principal;
        friend class User;
        friend class Account;

        //////////
        //  Construction/destruction (from DB type only)
    private:
        Object(Database * database, Oid oid);
        virtual ~Object();

        //////////
        //  tt3::db::api::IObject (general)
    public:
        virtual Database *  database() const override { return _database; }
        virtual Oid         oid() const override { return _oid; }
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
        const Oid           _oid;

        State               _state = State::New;
        int                 _referenceCount = 0;
        bool                _isLive = true;

        //  Helpers
        void                _ensureLive() const throws (DatabaseException);
        virtual void        _markDead();

        //////////
        //  Serialization
    private:
        virtual void        _serializeProperties(QDomElement & objectElement);
        virtual void        _serializeAggregations(QDomElement & parentElement);

        virtual void        _deserializeProperties(const QDomElement & objectElement) throws(ParseException);
        virtual void        _deserializeAggregations(const QDomElement & parentElement) ;

        //////////
        //  Validation
    private:
        virtual void        _validate(QSet<Object*> & validatedObjects) throws(DatabaseException);
    };
}

//  End of tt3-db-api/Object.hpp
