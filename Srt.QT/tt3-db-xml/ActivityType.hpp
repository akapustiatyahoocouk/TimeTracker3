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
    /// \class ActivityType tt3-db-xml/API.hpp
    /// \brief An activity type in an XML database.
    class TT3_DB_XML_PUBLIC ActivityType final
        :   public Object,
            public virtual tt3::db::api::IActivityType
    {
        CANNOT_ASSIGN_OR_COPY_CONSTRUCT(ActivityType)

        friend class Database;
        friend class Activity;

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
                            ) override;

        //////////
        //  tt3::db::api::IActivityType (properties)
    public:
        virtual auto    displayName(
                            ) const -> QString override;
        virtual void    setDisplayName(
                             const QString & displayName
                            ) override;
        virtual auto    description(
                            ) const -> QString override;
        virtual void    setDescription(
                                const QString & description
                            ) override;

        //////////
        //  tt3::db::api::IActivityType (associations)
    public:
        virtual auto    activities(
                            ) const -> tt3::db::api::Activities override;

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
                                QDomElement & objectElement
                            ) override;
        virtual void    _serializeAssociations(
                                QDomElement & objectElement
                            ) override;

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
