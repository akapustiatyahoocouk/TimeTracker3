//
//  tt3-db-xml/User.hpp - a named user
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
    /// \class User tt3-db-xml/API.hpp
    /// \brief A named user in an XML file database.
    class TT3_DB_XML_PUBLIC User final
        :   public Principal,
            public virtual tt3::db::api::IUser
    {
        CANNOT_ASSIGN_OR_COPY_CONSTRUCT(User)

        friend class Database;
        friend class Workload;
        friend class Account;

        //////////
        //  Construction/destruction (from DB type only)
    private:
        User(Database * database, tt3::db::api::Oid oid);
        virtual ~User();

        //////////
        //  tt3::db::api::IObject (life cycle)
    public:
        virtual void    destroy(
                            ) override;

        //////////
        //  tt3::db::api::IUser (properties)
    public:
        virtual auto    realName(
                            ) const -> QString override;
        virtual void    setRealName(
                                const QString & realName
                            ) override;
        virtual auto    inactivityTimeout(
                            ) const -> tt3::db::api::InactivityTimeout override;
        virtual void    setInactivityTimeout(
                                const tt3::db::api::InactivityTimeout & inactivityTimeout
                            ) override;
        virtual auto    uiLocale(
                            ) const -> tt3::db::api::UiLocale override;
        virtual void    setUiLocale(
                                const tt3::db::api::UiLocale & uiLocale
                            ) override;

        //////////
        //  tt3::db::api::IUser (associations)
    public:
        virtual auto    accounts(
                            ) const -> tt3::db::api::Accounts override;
        virtual auto    privateActivities(
                            ) const -> tt3::db::api::PrivateActivities override;
        virtual auto    privateActivitiesAndTasks(
                            ) const -> tt3::db::api::PrivateActivities override;
        virtual auto    privateTasks(
                            ) const -> tt3::db::api::PrivateTasks override;
        virtual auto    rootPrivateTasks(
                            ) const -> tt3::db::api::PrivateTasks override;
        virtual auto    permittedWorkloads(
                            ) const -> tt3::db::api::Workloads override;
        virtual void    setPermittedWorkloads(
                                const tt3::db::api::Workloads & workloads
                            ) override;
        virtual void    addPermittedWorkload(
                                tt3::db::api::IWorkload * workload
                            ) override;
        virtual void    removePermittedWorkload(
                                tt3::db::api::IWorkload * workload
                            ) override;

        //////////
        //  tt3::db::api::IUser  (life cycle)
    public:
        virtual auto    createAccount(
                                bool enabled,
                                const QStringList & emailAddresses,
                                const QString & login,
                                const QString & password,
                                tt3::db::api::Capabilities capabilities
                            ) -> tt3::db::api::IAccount * override;
        virtual auto    createPrivateActivity(
                                const QString & displayName,
                                const QString & description,
                                const tt3::db::api::InactivityTimeout & timeout,
                                bool requireCommentOnStart,
                                bool requireCommentOnFinish,
                                bool fullScreenReminder,
                                tt3::db::api::IActivityType * activityType,
                                tt3::db::api::IWorkload * workload
                            ) -> tt3::db::api::IPrivateActivity * override;
        virtual auto    createPrivateTask(
                                const QString & displayName,
                                const QString & description,
                                const tt3::db::api::InactivityTimeout & timeout,
                                bool requireCommentOnStart,
                                bool requireCommentOnFinish,
                                bool fullScreenReminder,
                                tt3::db::api::IActivityType * activityType,
                                tt3::db::api::IWorkload * workload,
                                bool completed,
                                bool requireCommentOnCompletion
                           ) -> tt3::db::api::IPrivateTask * override;

        //////////
        //  Implementation
    private:
        //  Properties
        QString         _realName;
        tt3::db::api::InactivityTimeout _inactivityTimeout;
        tt3::db::api::UiLocale          _uiLocale;
        //  Aggregations
        Accounts        _accounts;          //  count as "references"
        //  Associations
        Workloads       _permittedWorkloads;//  count as "references"

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

//  End of tt3-db-xml/User.hpp
