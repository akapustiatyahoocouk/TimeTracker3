//
//  tt3-db-xml/Task.hpp - a generic task
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
    /// \class Task tt3-db-xml/API.hpp
    /// \brief A generic task in an XML file database.
    class TT3_DB_XML_PUBLIC Task
        :   public virtual Activity,
            public virtual tt3::db::api::ITask
    {
        CANNOT_ASSIGN_OR_COPY_CONSTRUCT(Task)

        friend class PublicTask;

        //////////
        //  Construction/destruction (from DB type only)
    private:
        Task(Database * database, tt3::db::api::Oid oid);
        virtual ~Task();

        //////////
        //  tt3::db::api::ITask (properties)
    public:
        virtual bool    completed(
                            ) const override;
        virtual void    setCompleted(
                                bool completed
                            ) override;
        virtual bool    requireCommentOnCompletion(
                            ) const override;
        virtual void    setRequireCommentOnCompletion(
                                bool requireCommentOnCompletion
                            ) override;

        //////////
        //  Implementation
    private:
        //  Properties
        bool            _requireCommentOnCompletion;
        bool            _completed;

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

//  End of tt3-db-xml/Task.hpp
