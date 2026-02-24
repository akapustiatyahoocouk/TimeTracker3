//
//  tt3-db-sql/Project.hpp - a project
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
    /// \class Project tt3-db-sql/API.hpp
    /// \brief A project in an SQL database.
    class TT3_DB_SQL_PUBLIC Project final
        :   public Workload,
            public virtual tt3::db::api::IProject
    {
        TT3_CANNOT_ASSIGN_OR_COPY_CONSTRUCT(Project)

        friend class Database;

        //////////
        //  Construction/destruction (from DB type only)
    private:
        Project(Database * database, qint64 pk);
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
        //  Cached properties
        CachedProperty<bool>    _completed;

        CachedProperty<std::optional<qint64>>   _fkParent;

        virtual void    _invalidateCachedProperties() override;
        virtual void    _loadCachedProperties() override;
        void            _saveCompleted(bool completed);
        void            _saveFkParent(const std::optional<qint64> & fkParent);

        //  Helpers
        virtual bool    _siblingExists(const QString & displayName) const override;
        virtual void    _deleteCascade() override;  //  may throw
        bool            _childExists(const QString & displayName) const;    //  TODO use the same "...exists" instead of "find..." wherever possible
        void            _collectParentClosure(Projects & closure);
    };
}

//  End of tt3-db-sql/Project.hpp
