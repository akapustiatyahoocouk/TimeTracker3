//
//  tt3-db-xml/ActivityType.hpp - an activity type
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
    //  An activity type
    class TT3_DB_XML_PUBLIC ActivityType final
        :   public Object,
            public virtual tt3::db::api::IActivityType
    {
        CANNOT_ASSIGN_OR_COPY_CONSTRUCT(ActivityType)

        friend class Database;

        //////////
        //  Construction/destruction (from DB type only)
    private:
        ActivityType(
                Database * database,
                tt3::db::api::Oid oid
            );
        virtual ~ActivityType();

        //////////
        //  tt3::db::api::IObject (life cycle)
    public:
        virtual void    destroy(
            )
            throws(tt3::db::api::DatabaseException) override;

        //////////
        //  tt3::db::api::IActivityType (properties)
    public:
        virtual auto    displayName(
                            ) const
                            throws(tt3::db::api::DatabaseException)
                            -> QString override;
        virtual void    setDisplayName(
                             const QString & displayName
                            )
                            throws(tt3::db::api::DatabaseException) override;
        virtual auto    description(
                            ) const
                            throws(tt3::db::api::DatabaseException)
                            -> QString override;
        virtual void    setDescription(
                                const QString & description
                            )
                            throws(tt3::db::api::DatabaseException) override;

        //////////
        //  tt3::db::api::IActivityType (associations)
    public:
        virtual auto    activities(
                            ) const
                            throws(tt3::db::api::DatabaseException)
                            -> tt3::db::api::Activities override;

        //////////
        //  Implementation
    private:
        //  Properties
        QString         _displayName;
        QString         _description;
        //  Associations
        Activities      _activities;    //  count as "reference"s

        //  Helpers
        virtual void    _markDead() override;

        //////////
        //  Serialization
    private:
        virtual void    _serializeProperties(
                                QDomElement & objectElement
                            ) override;
        virtual void    _serializeAggregations(
                                QDomElement & parentElement
                            ) override;

        virtual void    _deserializeProperties(
                                const QDomElement & objectElement
                            ) throws(tt3::util::ParseException) override;
        virtual void    _deserializeAggregations(
                                const QDomElement & parentElement
                            ) throws(tt3::util::ParseException) override;

        //////////
        //  Validation
    private:
        virtual void    _validate(
                                QSet<Object*> & validatedObjects)
                            throws(tt3::db::api::DatabaseException) override;
    };
}

//  End of tt3-db-xml/ActivityType.hpp
