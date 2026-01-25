//
//  tt3-db-sql/Principal.hpp - a generic access control principal
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
    /// \class Principal tt3-db-sql/API.hpp
    /// \brief A generic access control principal in an SQL database.
    class TT3_DB_SQL_PUBLIC Principal
        :   public Object,
            public virtual tt3::db::api::IPrincipal
    {
        TT3_CANNOT_ASSIGN_OR_COPY_CONSTRUCT(Principal)

        friend class User;

        //////////
        //  Construction/destruction (from DB type only)
    private:
        Principal(Database * database, qint64 pk);
        virtual ~Principal();

        //////////
        //  tt3::db::api::IPrincipal (general)
    public:
        virtual bool    enabled(
                            ) const override;
        virtual void    setEnabled(
                                bool enabled
                            ) override;
        virtual auto    emailAddresses(
                            ) const -> QStringList override;
        virtual void    setEmailAddresses(
                                const QStringList & emailAddresses
                            ) override;

        //////////
        //  Implementation
    private:
        //  Cached properties
        CachedProperty<bool>        _enabled;
        CachedProperty<QStringList> _emailAddresses;

        virtual void    _invalidateCachedProperties() override;
        void            _saveEnabled(bool enabled);
        void            _saveEmailAddresses(const QStringList & emailAddresses);
    };
}

//  End of tt3-db-sql/Principal.hpp
