//
//  tt3-db-xml/Beneficiary.hpp - a generic Beneficiary
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
    /// \class Beneficiary tt3-db-xml/API.hpp
    /// \brief A generic beneficiary in an XML database.
    class TT3_DB_XML_PUBLIC Beneficiary
        :   public Object,
            public virtual tt3::db::api::IBeneficiary
    {
        TT3_CANNOT_ASSIGN_OR_COPY_CONSTRUCT(Beneficiary)

        friend class Database;
        friend class Workload;
        friend class Project;

        //////////
        //  Construction/destruction (from DB type only)
    private:
        Beneficiary(Database * database, tt3::db::api::Oid oid);
        virtual ~Beneficiary();

        //////////
        //  tt3::db::api::IBeneficiary (properties)
    public:
        virtual QString displayName(
                            ) const override;
        virtual void    setDisplayName(
                                const QString & displayName
                            ) override;
        virtual QString description(
                            ) const override;
        virtual void    setDescription(
                                const QString & description
                            ) override;

        //////////
        //  tt3::db::api::IBeneficiary (associations)
    public:
        virtual auto    workloads(
                            ) const -> tt3::db::api::Workloads override;
        virtual void    setWorkloads(
                                const tt3::db::api::Workloads & workloads
                            ) override;
        virtual void    addWorkload(
                                tt3::db::api::IWorkload * workload
                            ) override;
        virtual void    removeWorkload(
                                tt3::db::api::IWorkload * workload
                            ) override;

        //////////
        //  Implementation
    private:
        //  Properties
        QString         _displayName;
        QString         _description;
        //  Associations
        Workloads       _workloads; //  count as "references"

        //  Helpers
        bool            _siblingExists(const QString & displayName) const;
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
