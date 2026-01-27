//
//  tt3-db-sql/Transaction.cpp - tt3::db::sql::Transaction class implementation
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
#include "tt3-db-sql/API.hpp"
using namespace tt3::db::sql;

//////////
//  Construction/destruction
Transaction::Transaction(Database * database)
        :   _database(database)
{
    Q_ASSERT(_database != nullptr);

    _database->beginTransaction(); //  may throw
}

Transaction::~Transaction()
{
    if (!_committed && !_rollbacked)
    {
        try
        {
            _database->rollbackTransaction();   //  may throw
        }
        catch (const tt3::util::Exception & ex)
        {   //  Log, but suppress
            qCritical() << ex;
        }
        catch (const tt3::util::Error & ex)
        {   //  Log, but suppress
            qCritical() << ex;
        }
        catch (...)
        {   //  OOPS! Still, must suppress
        }
    }
}

//////////
//  Operations
void Transaction::commit()
{
    if (!_committed)
    {
        _database->commitTransaction();   //  may throw
        _committed = true;
    }
}

void Transaction::rollback()
{
    if (!_rollbacked)
    {
        _database->rollbackTransaction();   //  may throw
        _rollbacked = true;
    }
}

//  End of tt3-db-sql/Transaction.cpp
