//
//  tt3-db-xml/Activity.hpp - a generic activity
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
    /// \class Activity tt3-db-xml/API.hpp
    /// \brief A generic activity in an XML file database.
    class TT3_DB_XML_PUBLIC Activity
        :   public Object,
            public virtual tt3::db::api::IActivity
    {
        CANNOT_ASSIGN_OR_COPY_CONSTRUCT(Activity)

        friend class Database;
        friend class ActivityType;
        friend class PublicActivity;

        //////////
        //  Construction/destruction (from DB type only)
    private:
        Activity(Database * database, tt3::db::api::Oid oid);
        virtual ~Activity();

        //////////
        //  tt3::db::api::IActivity (general)
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
        virtual auto    timeout(
                            ) const -> tt3::db::api::InactivityTimeout override;
        virtual void    setTimeout(
                                const tt3::db::api::InactivityTimeout & timeout
                            ) override;
        virtual bool    requireCommentOnStart(
                            ) const override;
        virtual void    setRequireCommentOnStart(
                                bool requireCommentOnStart
                            ) override;
        virtual bool    requireCommentOnFinish(
                            ) const override;
        virtual void    setRequireCommentOnFinish(
                                bool requireCommentOnFinish
                            ) override;
        virtual bool    fullScreenReminder(
                            ) const override;
        virtual void    setFullScreenReminder(
                                bool fullScreenReminder
                            ) override;
        //////////
        //  tt3::db::api::IActivity (associations)
    public:
        virtual auto    activityType(
                            ) const -> tt3::db::api::IActivityType * override;
        virtual void    setActivityType(
                                tt3::db::api::IActivityType * activityType
                            ) override;
        virtual auto    workload(
                            ) const -> tt3::db::api::IWorkload * override;
        virtual void    setWorkload(
                                tt3::db::api::IWorkload * workload
                            ) override;
        virtual auto    works(
                            ) const -> tt3::db::api::Works override;
        virtual auto    events(
                            ) const -> tt3::db::api::Events override;

        //////////
        //  Implementation
    private:
        //  Properties
        QString         _displayName;
        QString         _description;
        tt3::db::api::InactivityTimeout _timeout;
        bool            _requireCommentOnStart = false;
        bool            _requireCommentOnFinish = false;
        bool            _fullScreenReminder = false;
        //  Associations
        ActivityType *  _activityType = nullptr;//  counts as "reference" unless nullptr
        Workload *      _workload = nullptr;    //  counts as "reference" uness nullptr
        Works           _works;     //  count as "references"
        Events          _events;    //  count as "references"

        //  Helpers
        virtual bool    _siblingExists(const QString & displayName) const = 0;

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
                            ) override; //  throws tt3::util::ParseException) overrid
        virtual void    _deserializeAggregations(
                                const QDomElement & objectElement
                            ) override; //  throws tt3::util::ParseException) overrid
        virtual void    _deserializeAssociations(
                                const QDomElement & objectElement
                            ) override;  //  throws tt3::util::ParseException

        //////////
        //  Validation
    private:
        virtual void    _validate(
                                Objects & validatedObjects
                            ) override; //  throws tt3::db::api::DatabaseException) overrid
    };
}

//  End of tt3-db-xml/Activity.hpp
