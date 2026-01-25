//
//  tt3-db-sql/Object.hpp - a generic object in an SQL database
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

namespace tt3::db::sql
{
    /// \class Object tt3-db-xml/API.hpp
    /// \brief A generic object in an SQL database.
    class TT3_DB_SQL_PUBLIC Object
        :   public virtual tt3::db::api::IObject
    {
        TT3_CANNOT_ASSIGN_OR_COPY_CONSTRUCT(Object)

        friend class Principal;
        friend class User;

        //////////
        //  Construction/destruction (from DB type only)
    private:
        Object(Database * database, qint64 pk);
        virtual ~Object();

        //////////
        //  tt3::db::api::IObject (general)
    public:
        virtual auto    database() const -> Database * override final { return _database; }
        virtual auto    oid() const -> tt3::db::api::Oid override final;
        virtual void    setOid(const tt3::db::api::Oid & oid) override final;
        virtual bool    isLive() const override final;

        //////////
        //  tt3::db::api::IObject (life cycle)
    public:
        virtual void    destroy() override final;

        //////////
        //  tt3::db::api::IObject (reference counting)
    public:
        virtual State   state() const override final;
        virtual int     referenceCount() const override final;
        virtual void    addReference() override final;
        virtual void    removeReference() override final;

        //////////
        //  Implementation
    private:
        Database *const _database;
        const qint64    _pk;

        //  Reference counting
        State           _state = State::New;
        int             _referenceCount = 0;
        bool            _isLive = true;

        //  Cached properties
        CachedProperty<tt3::db::api::Oid>   _oid;

        virtual void    _invalidateCachedProperties();
        virtual void    _loadCachedProperties() = 0;
        void            _saveOid(const tt3::db::api::Oid & oid);

        //  Helpers
        void            _ensureLive() const;
        void            _ensureLiveAndWritable() const;
        virtual void    _makeDead();
    };
}

//  End of tt3-db-sql/Object.hpp
