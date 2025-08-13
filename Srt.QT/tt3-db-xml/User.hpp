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
    //////////
    //  A named user
    class TT3_DB_XML_PUBLIC User final
        :   public Principal,
            public virtual tt3::db::api::IUser
    {
        CANNOT_ASSIGN_OR_COPY_CONSTRUCT(User)

        friend class Database;

        //////////
        //  Construction/destruction (from DB type only)
    protected:
        User(Database * database, Oid oid);
        virtual ~User();

        //////////
        //  tt3::db::api::IUser (properties)
    public:
        virtual QString     realName() const throws(DatabaseException) override;
        virtual void        setRealName(const QString & realName) throws(DatabaseException) override;
        virtual std::optional<tt3::util::TimeSpan>
                            inactivityTimeout() const throws(DatabaseException) override;
        virtual void        setInactivityTimeout(const std::optional<tt3::util::TimeSpan> & inactivityTimeout) throws(DatabaseException) override;
        virtual std::optional<QLocale>
                            uiLocale() const throws(DatabaseException) override;
        virtual void        setUiLocale(const std::optional<QLocale> & uiLocale) throws(DatabaseException) override;

        //////////
        //  Implementation
    private:
        //  Properties
        QString         _realName;
        std::optional<tt3::util::TimeSpan>  _inactivityTimeout;
        std::optional<QLocale>  _uiLocale;
        //  Associations
        Accounts        _accounts;  //  count as "references"
    };
}

//  End of tt3-db-api/User.hpp
