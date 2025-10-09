//
//  tt3-db-xml/Work.hpp - a work unit
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
    /// \class Work tt3-db-xml/API.hpp
    /// \brief A work unit in an XML database.
    class TT3_DB_XML_PUBLIC Work final
        :   public Object,
            public virtual tt3::db::api::IWork
    {
        CANNOT_ASSIGN_OR_COPY_CONSTRUCT(Work)

        friend class Database;
        friend class Account;

        //////////
        //  Construction/destruction (from DB type only)
    private:
        Work(Account * account, tt3::db::api::Oid oid);
        virtual ~Work();

        //////////
        //  tt3::db::api::IObject (life cycle)
    public:
        virtual void    destroy(
                            ) override;

        //////////
        //  tt3::db::api::IWork (properties)
    public:
        virtual auto    startedAt(
                            ) const -> QDateTime override;
        virtual auto    finishedAt(
                            ) const -> QDateTime override;

        //////////
        //  tt3::db::api::IWork (associations)
    public:
        virtual auto    account(
                            ) const -> tt3::db::api::IAccount * override;
        virtual auto    activity(
                            ) const -> tt3::db::api::IActivity * override;

        //////////
        //  Implementation
    private:
        //  Properties
        QDateTime       _startedAt;
        QDateTime       _finishedAt;
        //  Associations
        Account *       _account;   //  counts as "reference"
        Activity *      _activity;  //  counts as "reference"

        //  Helpers
        virtual void    _markDead() override;

        //////////
        //  Serialization
    private:
        virtual void    _serializeProperties(
                                QDomElement & objectElement
                            ) const override;
        virtual void    _serializeAggregations(
                                QDomElement & objectElement
                            ) const override;
        virtual void    _serializeAssociations(
                                QDomElement & objectElement
                            ) const override;

        virtual void    _deserializeProperties(
                                const QDomElement & objectElement
                            ) override; //  throws tt3::util::ParseException
        virtual void    _deserializeAggregations(
                                const QDomElement & objectElement
                            ) override; //  throws tt3::util::ParseException
        virtual void    _deserializeAssociations(
                                const QDomElement & objectElement
                            ) override;  //  throws tt3::util::ParseException

        //////////
        //  Validation
    private:
        virtual void    _validate(
                                Objects & validatedObjects
                            ) override; //  throws(tt3::db::api::DatabaseException
    };
}

//  End of tt3-db-xml/ActivityType.hpp
