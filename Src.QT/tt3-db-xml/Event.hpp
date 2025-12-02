//
//  tt3-db-xml/Event.hpp - an event
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
    /// \class Event tt3-db-xml/API.hpp
    /// \brief An event in an XML database.
    class TT3_DB_XML_PUBLIC Event final
        :   public Object,
            public virtual tt3::db::api::IEvent
    {
        TT3_CANNOT_ASSIGN_OR_COPY_CONSTRUCT(Event)

        friend class Database;
        friend class Account;
        friend class Activity;

        //////////
        //  Construction/destruction (from DB type only)
    private:
        Event(Account * account, tt3::db::api::Oid oid);
        virtual ~Event();

        //////////
        //  tt3::db::api::IEvent (properties)
    public:
        virtual auto    occurredAt(
                            ) const -> QDateTime override;
        virtual auto    summary(
                            ) const -> QString override;

        //////////
        //  tt3::db::api::IEvent (associations)
    public:
        virtual auto    account(
                            ) const -> tt3::db::api::IAccount * override;
        virtual auto    activities(
                            ) const -> tt3::db::api::Activities override;

        //////////
        //  Implementation
    private:
        //  Properties
        QDateTime       _occurredAt;
        QString         _summary;
        //  Associations
        Account *       _account;   //  counts as "reference"
        Activities      _activities;//  count as "references"

        //  Helpers
        virtual void    _makeDead() override;

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
