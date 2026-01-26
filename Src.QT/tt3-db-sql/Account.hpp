//
//  tt3-db-sql/Account.hpp - a login account
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

namespace tt3::db::sql
{
    /// \class Account tt3-db-sql/API.hpp
    /// \brief A login account in an SQL database.
    class TT3_DB_SQL_PUBLIC Account final
        :   public Principal,
            public virtual tt3::db::api::IAccount
    {
        TT3_CANNOT_ASSIGN_OR_COPY_CONSTRUCT(Account)

        //////////
        //  Construction/destruction (from DB type only)
    private:
        Account(Database * database, qint64 pk);
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
        //  Cached properties
        CachedProperty<QString>     _login;
        CachedProperty<QString>     _passwordHash;  //  SHA-1 uppercase hexstring
        CachedProperty<tt3::db::api::Capabilities>  _capabilities;

        virtual void    _invalidateCachedProperties() override;
        virtual void    _loadCachedProperties() override;
        void            _saveLogin(const QString & login);
        void            _savePasswordHash(const QString & passwordHash);
        void            _saveCapabilities(tt3::db::api::Capabilities  capabilities);

        //  Helpers
        virtual void    _makeDead() override;
    };
}

//  End of tt3-db-xml/Account.hpp
