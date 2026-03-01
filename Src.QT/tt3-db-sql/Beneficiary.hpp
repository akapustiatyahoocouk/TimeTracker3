//
//  tt3-db-sql/Beneficiary.hpp - a Beneficiary
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
    /// \class Beneficiary tt3-db-sql/API.hpp
    /// \brief A generic beneficiary in an SQL database.
    class TT3_DB_SQL_PUBLIC Beneficiary
        :   public Object,
            public virtual tt3::db::api::IBeneficiary
    {
        TT3_CANNOT_ASSIGN_OR_COPY_CONSTRUCT(Beneficiary)

        friend class Database;

        //////////
        //  Construction/destruction (from DB type only)
    private:
        Beneficiary(Database * database, qint64 pkd);
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
        //  Cached properties
        CachedProperty<QString> _displayName;
        CachedProperty<QString> _description;

        virtual void    _invalidateCachedProperties() override;
        virtual void    _loadCachedProperties() override;
        void            _saveDisplayName(const QString & displayName);
        void            _saveDescription(const QString & description);

        //  Helpers
        bool            _siblingExists(const QString & displayName) const;
        virtual void    _deleteCascade() override;  //  may throw
        virtual void    _removeFromDatabase() override; //  may throw
    };
}

//  End of tt3-db-sql/Beneficiary.hpp
