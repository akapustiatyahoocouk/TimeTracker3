//
//  tt3-db-sqlite3/DatabaseType.hpp - "SQLite3 database type" ADT
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

namespace tt3::db::sqlite3
{
    /// \class DatabaseType tt3-db-sqlite3/API.hpp
    /// \brief A "database type" that uses SQLite3 database.
    class TT3_DB_SQLITE3_PUBLIC DatabaseType final
        :   public virtual tt3::db::api::IDatabaseType
    {
        TT3_DECLARE_SINGLETON(DatabaseType)

        friend class DatabaseAddress;

        //////////
        //  Constants
    public:
        /// \brief
        ///     The preferred extension for TT3 SQLite3
        ///     database files; starts with '.'.
        inline static const QString    PreferredExtension = ".tt3-sqlite3";

        //////////
        //  tt3::db::api::IDatabaseType (general)
    public:
        virtual auto    mnemonic(
                            ) const ->tt3::util::Mnemonic override;
        virtual QString displayName(
                            ) const override;
        virtual QIcon   smallIcon(
                            ) const override;
        virtual QIcon   largeIcon(
                            ) const override;
        virtual bool    isOperational(
                            ) const override;
        virtual QString shortStatusReport(
                            ) const override;
        virtual QString fullStatusReport(
                            ) const override;
        virtual auto    validator(
                            ) const -> tt3::db::api::IValidator * override;

        //////////
        //  tt3::db::api::IDatabaseType (address handling)
    public:
        virtual auto    defaultDatabaseAddress(
                            ) const -> tt3::db::api::IDatabaseAddress * override;
        virtual auto    enterNewDatabaseAddress(
                                QWidget * parent
                            ) -> tt3::db::api::IDatabaseAddress * override;
        virtual auto    enterExistingDatabaseAddress(
                                QWidget * parent
                            ) -> tt3::db::api::IDatabaseAddress * override;
        virtual auto    parseDatabaseAddress(
                                const QString & externalForm
                            ) -> tt3::db::api::IDatabaseAddress * override;

        //////////
        //  tt3::db::api::IDatabaseType (databases)
    public:
        virtual auto    createDatabase(
                                tt3::db::api::IDatabaseAddress * address
                            ) -> tt3::db::api::IDatabase * override;
        virtual auto    openDatabase(
                                tt3::db::api::IDatabaseAddress * address,
                                tt3::db::api::OpenMode openMode
                            ) -> tt3::db::api::IDatabase * override;
        virtual void    destroyDatabase(
                                tt3::db::api::IDatabaseAddress * address
                            ) override;

        //////////
        //  Implementation
    private:
        tt3::db::api::IValidator *const _validator;

        //  Cache of known database addresses
        tt3::util::Mutex    _databaseAddressesGuard;
        QMap<QString, DatabaseAddress*> _databaseAddresses; //  key == full path

        //  The "dead" database, which adopts all still-referenced
        //  objects from the deleted Database instances until their
        //  reference counts drop to zero
        DatabaseAddress *const  _deadDatabaseAddress;
        Database *const         _deadDatabase;
    };
}

//  End of tt3-db-xml/DatabaseType.hpp
