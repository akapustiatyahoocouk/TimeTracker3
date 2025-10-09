//
//  tt3-db-xml/PrivateTask.hpp - a private task
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
    /// \class PrivateTask tt3-db-xml/API.hpp
    /// \brief A private task in an XML file database.
    class TT3_DB_XML_PUBLIC PrivateTask final
        :   public PrivateActivity,
            public Task,
            public virtual tt3::db::api::IPrivateTask
    {
        CANNOT_ASSIGN_OR_COPY_CONSTRUCT(PrivateTask)

        friend class Database;
        friend class User;

        //////////
        //  Construction/destruction (from DB type only)
    private:
        PrivateTask(User * owner, tt3::db::api::Oid oid);
        PrivateTask(PrivateTask * parent, tt3::db::api::Oid oid);
        virtual ~PrivateTask();

        //////////
        //  tt3::db::api::IObject (life cycle)
    public:
        virtual void    destroy(
            ) override;

        //////////
        //  tt3::db::api::IPrivateTask (associations)
    public:
        virtual auto    parent(
                            ) const -> IPrivateTask * override;
        virtual void    setParent(
                                IPrivateTask * parent
                            ) override;
        virtual auto    children(
                            ) const -> tt3::db::api::PrivateTasks override;

        //////////
        //  tt3::db::api::IPrivateTask (life cycle)
    public:
        virtual auto    createChild(
                                const QString & displayName,
                                const QString & description,
                                const tt3::db::api::InactivityTimeout & timeout,
                                bool requireCommentOnStart,
                                bool requireCommentOnStop,
                                bool fullScreenReminder,
                                tt3::db::api::IActivityType * activityType,
                                tt3::db::api::IWorkload * workload,
                                bool completed,
                                bool requireCommentOnCompletion
                            ) -> tt3::db::api::IPrivateTask * override;

        //////////
        //  Implementation
    private:
        //  Associations
        PrivateTask *   _parent;    //  counts as "reference" unless nullptr
        //  Aggregations
        PrivateTasks    _children;  //  count as "references"

        //  Helpers
        virtual bool    _siblingExists(const QString & displayName) const override;
        virtual void    _markDead() override;
        PrivateTask *   _findChild(const QString & displayName) const;

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

//  End of tt3-db-xml/PublicTask.hpp
