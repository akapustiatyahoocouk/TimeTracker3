//
//  tt3-db-xml/Workload.hpp - a generic workload
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
    /// \class Workload tt3-db-xml/API.hpp
    /// \brief A generic workload in an XML database.
    class TT3_DB_XML_PUBLIC Workload
        :   public Object,
            public virtual tt3::db::api::IWorkload
    {
        CANNOT_ASSIGN_OR_COPY_CONSTRUCT(Workload)

        friend class Database;
        friend class User;
        friend class Activity;

        //////////
        //  Construction/destruction (from DB type only)
    private:
        Workload(
                Database * database,
                tt3::db::api::Oid oid
            );
        virtual ~Workload();

        //////////
        //  tt3::db::api::IWorkload (properties)
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
        //  Implementation
    private:
        //  Properties
        QString         _displayName;
        QString         _description;
        //  Associations
        Beneficiaries   _beneficiaries; //  count as "references"
        Users           _assignedUsers; //  count as "references"
        Activities      _contributingActivities;    //  count as "references"

        //  Helpers
        virtual bool    _siblingExists(const QString & displayName) const = 0;
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
