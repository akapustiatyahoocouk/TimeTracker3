//
//  tt3-db-xml/Account.hpp - a login account
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
    /// \class Account tt3-db-xml/API.hpp
    /// \brief A login account in an XML file database.
    class TT3_DB_XML_PUBLIC Account final
        :   public Principal,
            public virtual tt3::db::api::IAccount
    {
        CANNOT_ASSIGN_OR_COPY_CONSTRUCT(Account)

        friend class Database;
        friend class User;
        friend class Activity;
        friend class Work;
        friend class Event;

        //////////
        //  Construction/destruction (from DB type only)
    private:
        Account(
            User * user,
            tt3::db::api::Oid oid);
        virtual ~Account();

        //////////
        //  tt3::db::api::IAccount (properties)
    public:
        virtual QString login(
                            ) const override;
        virtual void    setLogin(
                                const QString & login
                            ) override;
        virtual QString passwordHash(
                            ) const override;
        virtual void    setPassword(
                                const QString & password
                            ) override;
        virtual auto    capabilities(
                            ) const -> tt3::db::api::Capabilities override;
        virtual void    setCapabilities(
                                tt3::db::api::Capabilities capabilities
                            ) override;

        //////////
        //  tt3::db::api::IAccount (associations)
    public:
        virtual auto    user(
                            ) const -> tt3::db::api::IUser * override;
        virtual auto    quickPicksList(
                            ) const -> QList<tt3::db::api::IActivity*> override;
        virtual void    setQuickPicksList(
                                const QList<tt3::db::api::IActivity*> & quickPicksList
                            ) override;
        virtual auto    works(
                            ) const -> tt3::db::api::Works override;
        virtual auto    works(
                                const QDateTime & from,
                                const QDateTime & to
                            ) const -> tt3::db::api::Works override;
        virtual auto    events(
                            ) const -> tt3::db::api::Events override;
        virtual auto    events(
                                const QDateTime & from,
                                const QDateTime & to
                            ) const -> tt3::db::api::Events override;

        //////////
        //  tt3::db::api::IAccount (life cycle)
    public:
        virtual auto    createWork(
                                const QDateTime & startedAt,
                                const QDateTime & finishedAt,
                                tt3::db::api::IActivity * activity
                            ) -> tt3::db::api::IWork * override;
        virtual auto    createEvent(
                                const QDateTime & occurredAt,
                                const QString & summary,
                                const tt3::db::api::Activities & activities
                            ) -> tt3::db::api::IEvent * override;
        //////////
        //  Implementation
    private:
        //  Properties
        QString         _login;
        QString         _passwordHash;  //  SHA-1 uppercase hexstring
        tt3::db::api::Capabilities  _capabilities;
        //  Aggregations
        Works           _works;         //  count as "reference"
        Events          _events;        //  count as "reference"
        //  Associations
        User *          _user;          //  counts as "reference"
        QList<Activity*>_quickPicksList; //  count as "reference"

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
                             ) override; //  throws tt3::db::api::DatabaseException
    };
}

//  End of tt3-db-xml/Account.hpp
