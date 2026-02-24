//
//  tt3-db-sql/Workload.hpp - a generic workload
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
    /// \class Workload tt3-db-sql/API.hpp
    /// \brief A generic workload in an SQL database.
    class TT3_DB_SQL_PUBLIC Workload
        :   public Object,
            public virtual tt3::db::api::IWorkload
    {
        TT3_CANNOT_ASSIGN_OR_COPY_CONSTRUCT(Workload)

        friend class Database;
        friend class WorkStream;

        //////////
        //  Construction/destruction (from DB type only)
    private:
        Workload(Database * database, qint64 pk);
        virtual ~Workload();

        //////////
        //  tt3::db::api::IWorkload (properties)
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
        //  tt3::db::api::IWorkload (associations)
    public:
        virtual auto    contributingActivities(
                            ) const -> tt3::db::api::Activities override;
        virtual auto    beneficiaries(
                            ) const -> tt3::db::api::Beneficiaries override;
        virtual void    setBeneficiaries(
                                const tt3::db::api::Beneficiaries & beneficiaries
                            ) override;
        virtual void    addBeneficiary(
                                tt3::db::api::IBeneficiary * beneficiary
                            ) override;
        virtual void    removeBeneficiary(
                                tt3::db::api::IBeneficiary * beneficiary
                            ) override;
        virtual auto    assignedUsers(
                            ) const -> tt3::db::api::Users override;
        virtual void    setAssignedUsers(
                                const tt3::db::api::Users & users
                            ) override;
        virtual void    addAssignedUser(
                                tt3::db::api::IUser * user
                            ) override;
        virtual void    removeAssignedUser(
                                tt3::db::api::IUser * user
                            ) override;

        //////////
        //  Implementation
    private:
        //  Cached properties
        CachedProperty<QString> _displayName;
        CachedProperty<QString> _description;

        virtual void    _invalidateCachedProperties() override;
        void            _saveDisplayName(const QString & displayName);
        void            _saveDescription(const QString & description);

        //  Helpers
        virtual bool    _siblingExists(const QString & displayName) const = 0;
        virtual void    _deleteCascade() override;  //  may throw
        virtual void    _removeFromDatabase() override; //  may throw
        virtual QString _tableName() const override { return "workloads"; }
    };
}

//  End of tt3-db-sql/Workload.hpp
