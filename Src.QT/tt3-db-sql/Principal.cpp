//
//  tt3-db-sql/Principal.cpp - tt3::db::sql::Principal class implementation
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
//  Construction/destruction (from DB type only)
Principal::Principal(Database * database, qint64 pk)
    :   Object(database, pk),
        //  Cached properties
        _enabled([this] { _loadCachedProperties(); }),
        _emailAddresses([this] { _loadCachedProperties(); })
{
}

Principal::~Principal()
{
}

//////////
//  tt3::db::api::IPrincipal (general)
bool Principal::enabled() const
{
    tt3::util::Lock _(_database->guard);
    _ensureLive();  //  may throw

    return _enabled;    //  cache load may throw
}

void Principal::setEnabled(
        bool enabled
    )
{
    tt3::util::Lock _(_database->guard);
    _ensureLiveAndWritable();   //  may throw

    if (enabled != _enabled)    //  cache load may throw
    {   //  Make the change...
        //  TODO wrap database access into a transaction
        _saveEnabled(enabled);  //  may throw
        _enabled = enabled;
        //  ...schedule change notifications...
        _database->_changeNotifier.post(
            new tt3::db::api::ObjectModifiedNotification(
                _database, type(), _oid));
        //  TODO post change notification to the database
        //  ...and we're done
    }
}

auto Principal::emailAddresses(
    ) const -> QStringList
{
    tt3::util::Lock _(_database->guard);
    _ensureLive();  //  may throw

    return _emailAddresses; //  cache load may throw
}

void Principal::setEmailAddresses(
        const QStringList & emailAddresses
    )
{
    tt3::util::Lock _(_database->guard);
    _ensureLiveAndWritable();   //  may throw

    //  Validate parameters
    if (!_database->validator()->principal()->isValidEmailAddresses(emailAddresses))
    {
        throw tt3::db::api::InvalidPropertyValueException(
            type(),
            "emailAddresses",
            emailAddresses.join(','));
    }

    if (emailAddresses != _emailAddresses)  //  cache load may throw
    {   //  Make the change...
        //  TODO wrap database access into a transaction
        _saveEmailAddresses(emailAddresses);    //  may throw
        _emailAddresses = emailAddresses;
        //  ...schedule change notifications...
        _database->_changeNotifier.post(
            new tt3::db::api::ObjectModifiedNotification(
                _database, type(), _oid));
        //  TODO post change notification to the database
        //  ...and we're done
    }
}

//////////
//  Cached properties
void Principal::_invalidateCachedProperties()
{
    Object::_invalidateCachedProperties();
    _enabled.invalidate();
    _emailAddresses.invalidate();
}

void Principal::_saveEnabled(bool enabled)
{
    Q_ASSERT(_database->guard.isLockedByCurrentThread());

    std::unique_ptr<Statement> stat
    {   _database->createStatement(
            "UPDATE [" + _tableName() + "]"
            "   SET [enabled] = ?"
            " WHERE [pk] = ?") };
    stat->setBoolParameter(0, enabled);
    stat->setIntParameter(1, _pk);
    auto affectedRows = stat->execute();    //  may throw
    if (affectedRows == 0)
    {   //  OOPS! Row since deleted!
        _makeDead();
        throw tt3::db::api::InstanceDeadException();
    }
}

void Principal::_saveEmailAddresses(const QStringList & emailAddresses)
{
    throw tt3::util::NotImplementedError();
}

//  End of tt3-db-sql/Principal.cpp
