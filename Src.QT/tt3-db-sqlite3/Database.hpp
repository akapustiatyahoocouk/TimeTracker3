//
//  tt3-db-sqlite3/Database.hpp - "SQLite3 database"
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
    /// \class Database tt3-db-sqlite3/API.hpp
    /// \brief A database stored in an SQLite3 file.
    class TT3_DB_SQLITE3_PUBLIC Database final
        :   public tt3::db::sql::Database
    {
        TT3_CANNOT_ASSIGN_OR_COPY_CONSTRUCT(Database)

        friend class DatabaseType;

        //////////
        //  Construction/destruction
    private:
        enum _OpenMode
        {
            _Create,
            _OpenReadOnly,
            _OpenReadWrite,
            _Dead
        };
        Database(DatabaseAddress * address,
                 _OpenMode openMode);   //  may throw
    public:
        virtual ~Database();    //  closes database if still open

        //////////
        //  tt3::db::api::IDatabase (general)
    public:
        virtual auto    type(
                            ) const -> DatabaseType * override;
        virtual auto    address(
                            ) const -> DatabaseAddress * override;
        virtual auto    validator(
                            ) const -> tt3::db::api::IValidator * override;
        virtual bool    isOpen() const override;
        virtual bool    isReadOnly() const override;
        virtual void    close() override;
        virtual void    refresh() override;

        //////////
        //  tt3::db::sql::Database (database specifics)
    protected:
        virtual bool    isKeyword(const QString & word) const override;
        virtual QString quoteIdentifier(const QString & identifier) const override;
        virtual qint64  executeInsert(const QString & sql) override;
        virtual qint64  executeUpdate(const QString & sql) override;
        virtual qint64  executeDelete(const QString & sql) override;
        virtual void    beginTransaction() override;
        virtual void    commitTransaction() override;
        virtual void    rollbackTransaction() override;
        virtual bool    isTransactionInProgress() const override;
        virtual auto    executeSelect(const QString & sql) -> tt3::db::sql::ResultSet * override;
        virtual void    execute(const QString & sql) override;

        virtual void    ensureOpen() const override;
        virtual void    ensureOpenAndWritable() const override;

        //////////
        //  Implementation
    private:
        DatabaseAddress *const          _address;   //  counts as a "reference"
        tt3::db::api::IValidator *const _validator;
        ::sqlite3 *     _connection = nullptr;  //  nullptr == database is closed
        bool            _isReadOnly = false;    //  not "const" - will be faked as "false" during close()
        bool            _transactionInProgress = false;

        const QSet<QString> _keywords;  //  all-uppercase

        //  Helpers
        static int      _selectCallback(void * cbData, int argc, char ** argv,char ** colNames);
    };
}

//  End of tt3-db-sqlite3/Database.hpp
