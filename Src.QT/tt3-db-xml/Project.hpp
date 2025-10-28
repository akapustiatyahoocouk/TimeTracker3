//
//  tt3-db-xml/Project.hpp - a project
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
    /// \class Project tt3-db-xml/API.hpp
    /// \brief A project in an XML file database.
    class TT3_DB_XML_PUBLIC Project final
        :   public Workload,
            public virtual tt3::db::api::IProject
    {
        CANNOT_ASSIGN_OR_COPY_CONSTRUCT(Project)

        friend class Database;

        //////////
        //  Construction/destruction (from DB type only)
    private:
        Project(Database * database, tt3::db::api::Oid oid);
        Project(Project * parent, tt3::db::api::Oid oid);
        virtual ~Project();

        //////////
        //  tt3::db::api::IProject (properties)
    public:
        virtual bool    completed(
                            ) const override;

        virtual void    setCompleted(
                                bool completed
                            ) override;

        //////////
        //  tt3::db::api::IProject (associations)
    public:
        virtual auto    parent(
                            ) const -> tt3::db::api::IProject * override;
        virtual void    setParent(
                                tt3::db::api::IProject * parent
                            ) override;
        virtual auto    children(
                            ) const -> tt3::db::api::Projects override;

        //////////
        //  tt3::db::api::IProject (life cycle)
    public:
        virtual auto    createChild(
                                const QString & displayName,
                                const QString & description,
                                const tt3::db::api::Beneficiaries & beneficiaries,
                                bool completed
                            ) -> tt3::db::api::IProject * override;

        //////////
        //  Implementation
    private:
        //  Properties
        bool            _completed;
        //  Associations
        Project *       _parent;    //  counts as "reference" unless nullptr
        //  Aggregations
        Projects        _children;  //  count as "references"

        //  Helpers
        virtual bool    _siblingExists(const QString & displayName) const override;
        virtual void    _makeDead() override;
        Project *       _findChild(const QString & displayName) const;
        void            _collectParentClosure(Projects & closure);

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
                            ) override; //  throws tt3::util::ParseException)
        virtual void    _deserializeAggregations(
                                const QDomElement & objectElement
                            ) override; //  throws tt3::util::ParseException)
        virtual void    _deserializeAssociations(
                                const QDomElement & objectElement
                            ) override;  //  throws tt3::util::ParseException

        //////////
        //  Validation
    private:
        virtual void    _validate(  //  throws tt3::db::api::DatabaseException
                                Objects & validatedObjects
                            ) override;
    };
}

//  End of tt3-db-xml/Project.hpp
