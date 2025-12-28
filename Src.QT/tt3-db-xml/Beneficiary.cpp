//
//  tt3-db-xml/Beneficiary.cpp - tt3::db::xml::Beneficiary class implementation
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
#include "tt3-db-xml/API.hpp"
using namespace tt3::db::xml;

//////////
//  Construction/destruction (from DB type only)
Beneficiary::Beneficiary(
        Database * database,
        tt3::db::api::Oid oid
    ) : Object(database, oid)
{
    //  Register User with parent
    _database->_beneficiaries.insert(this);
    this->addReference();
}

Beneficiary::~Beneficiary()
{
    Q_ASSERT(!_database->_beneficiaries.contains(this));
    Q_ASSERT(_workloads.isEmpty());
}

//////////
//  tt3::db::api::IBeneficiary (properties)
QString Beneficiary::displayName() const
{
    tt3::util::Lock _(_database->_guard);
    _ensureLive();  //  may throw
    //  We assume database is consistent since last change

    return _displayName;
}

void Beneficiary::setDisplayName(
        const QString & displayName
    )
{
    tt3::util::Lock _(_database->_guard);
    _ensureLiveAndWritable();   //  may throw
#ifdef Q_DEBUG
    _database->_validate(); //  may throw
#endif

    //  Validate parameters
    if (!_database->_validator->beneficiary()->isValidDisplayName(displayName))
    {
        throw tt3::db::api::InvalidPropertyValueException(
            type(),
            "displayName",
            displayName);
    }
    if (displayName != _displayName)
    {   //  Make the change (but no duplication)...
        if (_database->_findBeneficiary(displayName) != nullptr)
        {
            throw tt3::db::api::AlreadyExistsException(
                type(),
                "displayName",
                displayName);
        }
        _displayName = displayName;
        _database->_markModified();
        //  ...schedule change notifications...
        _database->_changeNotifier.post(
            new tt3::db::api::ObjectModifiedNotification(
                _database, type(), _oid));
        //  ...and we're done
#ifdef Q_DEBUG
        _database->_validate(); //  may throw
#endif
    }
}

QString Beneficiary::description() const
{
    tt3::util::Lock _(_database->_guard);
    _ensureLive();  //  may throw
    //  We assume database is consistent since last change

    return _description;
}

void Beneficiary::setDescription(
        const QString & description
    )
{
    tt3::util::Lock _(_database->_guard);
    _ensureLiveAndWritable();   //  may throw
#ifdef Q_DEBUG
    _database->_validate(); //  may throw
#endif

    //  Validate parameters
    if (!_database->_validator->beneficiary()->isValidDescription(description))
    {
        throw tt3::db::api::InvalidPropertyValueException(
            type(),
            "description",
            description);
    }
    if (description != _description)
    {   //  Make the change...
        _description = description;
        _database->_markModified();
        //  ...schedule change notifications...
        _database->_changeNotifier.post(
            new tt3::db::api::ObjectModifiedNotification(
                _database, type(), _oid));
        //  ...and we're done
#ifdef Q_DEBUG
        _database->_validate(); //  may throw
#endif
    }
}

//////////
//  tt3::db::api::IBeneficiary (associations)
auto Beneficiary::workloads(
    ) const -> tt3::db::api::Workloads
{
    tt3::util::Lock _(_database->_guard);
    _ensureLive();  //  may throw
    //  We assume database is consistent since last change

    return tt3::db::api::Workloads(_workloads.cbegin(), _workloads.cend());
}

void Beneficiary::setWorkloads(
        const tt3::db::api::Workloads & workloads
    )
{
    tt3::util::Lock _(_database->_guard);
    _ensureLiveAndWritable();   //  may throw
#ifdef Q_DEBUG
    _database->_validate(); //  may throw
#endif

    //  Validate parameters
    if (workloads.contains(nullptr))
    {
        throw tt3::db::api::InvalidPropertyValueException(
            type(),
            "workloads",
            nullptr);
    }

    Workloads xmlWorkloads =
        tt3::util::transform(
            workloads,
            [&](auto w)
            {
                Q_ASSERT(w != nullptr); //  should have been caught earlier!
                auto xmlWorkload = dynamic_cast<Workload*>(w);
                if (xmlWorkload == nullptr ||
                    xmlWorkload->_database != this->_database ||
                    !xmlWorkload->_isLive)
                {   //  OOPS!
                    throw tt3::db::api::IncompatibleInstanceException(w->type());
                }
                return xmlWorkload;
            });
    if (xmlWorkloads != _workloads)
    {   //  Make the changes
        Workloads addedWorkloads = xmlWorkloads - _workloads;
        Workloads removedWorkloads = _workloads - xmlWorkloads;
        //  link the added workloads...
        for (Workload * xmlWorkload : std::as_const(addedWorkloads))
        {
            _workloads.insert(xmlWorkload);
            xmlWorkload->_beneficiaries.insert(this);
            xmlWorkload->addReference();
            this->addReference();
        }
        //  ...un-link the removed workloads...
        for (Workload * xmlWorkload : std::as_const(removedWorkloads))
        {
            _workloads.remove(xmlWorkload);
            xmlWorkload->_beneficiaries.remove(this);
            xmlWorkload->removeReference();
            this->removeReference();
        }
        //  ...ensure the changes are saved...
        _database->_markModified();
        //  ...schedule change notifications...
        _database->_changeNotifier.post(
            new tt3::db::api::ObjectModifiedNotification(
                _database, type(), _oid));
        for (Workload * xmlWorkload : addedWorkloads + removedWorkloads)
        {
            _database->_changeNotifier.post(
                new tt3::db::api::ObjectModifiedNotification(
                    _database, xmlWorkload->type(), xmlWorkload->_oid));
        }
        //  ...and we're done
#ifdef Q_DEBUG
        _database->_validate(); //  may throw
#endif
    }
}

void Beneficiary::addWorkload(
        tt3::db::api::IWorkload * workload
    )
{
    tt3::util::Lock _(_database->_guard);
    _ensureLiveAndWritable();   //  may throw
#ifdef Q_DEBUG
    _database->_validate(); //  may throw
#endif

    //  Validate parameters
    if (workload == nullptr)
    {
        throw tt3::db::api::InvalidPropertyValueException(
            type(),
            "workload",
            nullptr);
    }

    Workload * xmlWorkload = dynamic_cast<Workload*>(workload);
    if (xmlWorkload == nullptr ||
        xmlWorkload->_database != this->_database ||
        !xmlWorkload->_isLive)
    {   //  OOPS!
        throw tt3::db::api::IncompatibleInstanceException(workload->type());
    }
    if (!_workloads.contains(xmlWorkload))
    {   //  Make the changes
        Q_ASSERT(!xmlWorkload->_beneficiaries.contains(this));
        _workloads.insert(xmlWorkload);
        xmlWorkload->_beneficiaries.insert(this);
        xmlWorkload->addReference();
        this->addReference();
        //  ...ensure the changes are saved...
        _database->_markModified();
        //  ...schedule change notifications...
        _database->_changeNotifier.post(
            new tt3::db::api::ObjectModifiedNotification(
                _database, type(), _oid));
        _database->_changeNotifier.post(
            new tt3::db::api::ObjectModifiedNotification(
                _database, xmlWorkload->type(), xmlWorkload->_oid));
        //  ...and we're done
#ifdef Q_DEBUG
        _database->_validate(); //  may throw
#endif
    }
}

void Beneficiary::removeWorkload(
        tt3::db::api::IWorkload * workload
    )
{
    tt3::util::Lock _(_database->_guard);
    _ensureLiveAndWritable();   //  may throw
#ifdef Q_DEBUG
    _database->_validate(); //  may throw
#endif

    //  Validate parameters
    if (workload == nullptr)
    {
        throw tt3::db::api::InvalidPropertyValueException(
            type(),
            "workload",
            nullptr);
    }

    Workload * xmlWorkload = dynamic_cast<Workload*>(workload);
    if (xmlWorkload == nullptr ||
        xmlWorkload->_database != this->_database ||
        !xmlWorkload->_isLive)
    {   //  OOPS!
        throw tt3::db::api::IncompatibleInstanceException(workload->type());
    }
    if (_workloads.contains(xmlWorkload))
    {   //  Make the changes
        Q_ASSERT(xmlWorkload->_beneficiaries.contains(this));
        _workloads.remove(xmlWorkload);
        xmlWorkload->_beneficiaries.remove(this);
        xmlWorkload->removeReference();
        this->removeReference();
        //  ...ensure the changes are saved...
        _database->_markModified();
        //  ...schedule change notifications...
        _database->_changeNotifier.post(
            new tt3::db::api::ObjectModifiedNotification(
                _database, type(), _oid));
        _database->_changeNotifier.post(
            new tt3::db::api::ObjectModifiedNotification(
                _database, xmlWorkload->type(), xmlWorkload->_oid));
        //  ...and we're done
#ifdef Q_DEBUG
        _database->_validate(); //  may throw
#endif
    }
}

//////////
//  Implementation helpers
void Beneficiary::_makeDead()
{
    Q_ASSERT(_database->_guard.isLockedByCurrentThread());
    Q_ASSERT(_isLive);

    //  Break associations
    for (Workload * workload : _workloads.values())
    {
        Q_ASSERT(workload->_beneficiaries.contains(this));
        workload->_beneficiaries.remove(this);
        _workloads.remove(workload);
        this->removeReference();
        workload->removeReference();
    }
    _workloads.clear();

    //  Remove from "live" caches
    Q_ASSERT(_database->_beneficiaries.contains(this));
    _database->_beneficiaries.remove(this);
    this->removeReference();

    //  The rest is up to the base class
    Object::_makeDead();
}

bool Beneficiary::_siblingExists(
        const QString & displayName
    ) const
{
    Q_ASSERT(_database->_guard.isLockedByCurrentThread());

    Beneficiary * sibling = _database->_findBeneficiary(displayName);
    return sibling != nullptr && sibling != this;
}

//////////
//  Serialization
void Beneficiary::_serializeProperties(
        QDomElement & objectElement
    ) const
{
    Object::_serializeProperties(objectElement);

    objectElement.setAttribute("DisplayName", _displayName);
    objectElement.setAttribute("Description", _description);
}

void Beneficiary::_serializeAggregations(
        QDomElement & objectElement
    ) const
{
    Object::_serializeAggregations(objectElement);
}

void Beneficiary::_serializeAssociations(
        QDomElement & objectElement
    ) const
{
    Object::_serializeAssociations(objectElement);

    _database->_serializeAssociation(
        objectElement,
        "Workloads",
        _workloads);
}

void Beneficiary::_deserializeProperties(
        const QDomElement & objectElement
    )
{
    Object::_deserializeProperties(objectElement);

    _displayName = objectElement.attribute("DisplayName");
    _description = objectElement.attribute("Description");
}

void Beneficiary::_deserializeAggregations(
        const QDomElement & objectElement
    )
{
    Object::_deserializeAggregations(objectElement);
}

void Beneficiary::_deserializeAssociations(
        const QDomElement & objectElement
    )
{
    Object::_deserializeAssociations(objectElement);

    _database->_deserializeAssociation(
        objectElement,
        "Workloads",
        _workloads);
}

//////////
//  Validation
void Beneficiary::_validate(
        Objects & validatedObjects
    )
{
    Object::_validate(validatedObjects);

    //  Validate properties
    if (!_database->_validator->beneficiary()->isValidDisplayName(_displayName))
    {   //  OOPS!
        throw tt3::db::api::DatabaseCorruptException(_database->_address);
    }
    if (!_database->_validator->beneficiary()->isValidDescription(_description))
    {   //  OOPS!
        throw tt3::db::api::DatabaseCorruptException(_database->_address);
    }

    //  Validate aggregations

    //  Validate associations
    for (Workload * workload : std::as_const(_workloads))
    {
        if (workload == nullptr ||
            workload->_database != this->_database ||
            !workload->_isLive ||
            !workload->_beneficiaries.contains(this))
        {   //  OOPS!
            throw tt3::db::api::DatabaseCorruptException(_database->_address);
        }
    }
}

//  End of tt3-db-xml/Beneficiary.cpp
