//
//  tt3-db-sql/Activity.hpp - a generic activity
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
    /// \class Activity tt3-db-xml/API.hpp
    /// \brief A generic activity in an SQL database.
    class TT3_DB_SQL_PUBLIC Activity
        :   public Object,
            public virtual tt3::db::api::IActivity
    {
        TT3_CANNOT_ASSIGN_OR_COPY_CONSTRUCT(Activity)

        friend class Database;
        friend class User;
        friend class PublicActivity;
        friend class PrivateActivity;
        friend class Task;
        friend class PublicTask;
        friend class PrivateTask;

        //////////
        //  Construction/destruction (from DB type only)
    private:
        Activity(Database * database, qint64 pk);
        virtual ~Activity();

        //////////
        //  tt3::db::api::IActivity (general)
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
        virtual bool    requireCommentOnStop(
                            ) const override;
        virtual void    setRequireCommentOnStop(
                                bool requireCommentOnStop
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
        //  Cached properties
        CachedProperty<QString> _displayName;
        CachedProperty<QString> _description;
        CachedProperty<tt3::db::api::InactivityTimeout> _timeout;
        CachedProperty<bool>    _requireCommentOnStart;
        CachedProperty<bool>    _requireCommentOnStop;
        CachedProperty<bool>    _fullScreenReminder;

        CachedProperty<std::optional<qint64>>   _fkActivityType;
        CachedProperty<std::optional<qint64>>   _fkWorkload;

        virtual void    _invalidateCachedProperties() override;
        void            _saveDisplayName(const QString & displayName);
        void            _saveDescription(const QString & description);
        void            _saveTimeout(const tt3::db::api::InactivityTimeout & timeout);
        void            _saveRequireCommentOnStart(bool requireCommentOnStart);
        void            _saveRequireCommentOnStop(bool requireCommentOnStop);
        void            _saveFullScreenReminder(bool fullScreenReminder);
        void            _saveFkActivityType(const std::optional<qint64> & fkActivityType);
        void            _saveFkWorkload(const std::optional<qint64> & fkWorkload);

        /*  TODO
        Workload *      _workload = nullptr;    //  counts as "reference" uness nullptr
        Works           _works;         //  count as "references"
        Events          _events;        //  count as "references"
        */

        //  Helpers
        virtual bool    _siblingExists(const QString & displayName) const = 0;
        virtual void    _deleteCascade() override;  //  may throw
        virtual void    _removeFromDatabase() override; //  may throw
    };
}

//  End of tt3-db-sql/Activity.hpp
