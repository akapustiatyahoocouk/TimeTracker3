//
//  tt3-db-xml/DatabaseType.hpp - "XML file database type" ADT
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
    //  A "database type" that stores the entire
    //  database in an XML file. Only one connection
    //  to sucj a "database" is permitted at any
    //  iven time.
    class TT3_DB_XML_PUBLIC DatabaseType final
        :   public virtual tt3::db::api::IDatabaseType
    {
        DECLARE_SINGLETON(DatabaseType)

        friend class DatabaseAddress;

        //////////
        //  tt3::db::api::IDatabaseType (general)
    public:
        virtual QString     mnemonic() const override;
        virtual QString     displayName() const override;
        virtual QIcon       smallIcon() const override;
        virtual QIcon       largeIcon() const override;
        virtual bool        isOperational() const override;
        virtual QString     shortStatusReport() const override;
        virtual QString     fullStatusReport() const override;
        virtual tt3::db::api::IValidator *  validator() const override;

        //////////
        //  tt3::db::api::IDatabaseType (address handling)
    public:
        virtual tt3::db::api::IDatabaseAddress *  defaultDatabaseAddress() const override;
        virtual tt3::db::api::IDatabaseAddress *  enterNewDatabaseAddress(QWidget * parent) override;
        virtual tt3::db::api::IDatabaseAddress *  enterExistingDatabaseAddress(QWidget * parent) override;
        virtual tt3::db::api::IDatabaseAddress *  parseDatabaseAddress(const QString & externalForm) throws(tt3::db::api::DatabaseException) override;

        //////////
        //  tt3::db::api::IDatabaseType (databases)
    public:
        virtual tt3::db::api::IDatabase *   createDatabase(tt3::db::api::IDatabaseAddress * address) throws(tt3::db::api::DatabaseException) override;
        virtual tt3::db::api::IDatabase *   openDatabase(tt3::db::api::IDatabaseAddress * address) throws(tt3::db::api::DatabaseException) override;
        virtual void        destroyDatabase(tt3::db::api::IDatabaseAddress * address) throws(tt3::db::api::DatabaseException) override;

        //////////
        //  Implementation
    private:
        const QIcon         _smallIcon;
        const QIcon         _largeIcon;
        tt3::db::api::IValidator *const _validator;

        //  Cache of known database addresses
        tt3::util::Mutex    _databaseAddressesGuard;
        QMap<QString, DatabaseAddress*> _databaseAddresses; //  key == full path
        //  TODO how do we recycled Old DatabaseAddress instances ? Queue of Olds ?
    };
}

//  End of tt3-db-xml/DatabaseType.hpp
