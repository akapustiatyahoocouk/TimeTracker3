//
//  tt3-db-xml/Workload.cpp - tt3::db::xml::Workload class implementation
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
Workload::Workload(
        Database * database,
        tt3::db::api::Oid oid
    ) : Object(database, oid)
{
}

Workload::~Workload()
{
}

//////////
//  tt3::db::api::IWorkload (properties)
QString Workload::displayName() const
{
    tt3::util::Lock lock(_database->_guard);
    _ensureLive();  //  may throw
    //  We assume database is consistent since last change

    return _displayName;
}

void Workload::setDisplayName(
        const QString & displayName
    )
{
    tt3::util::Lock lock(_database->_guard);
    _ensureLiveAndWritable();   //  may throw
#ifdef Q_DEBUG
    _database->_validate(); //  may throw
#endif

    //  Validate parameters
    if (!_database->_validator->workload()->isValidDisplayName(displayName))
    {
        throw tt3::db::api::InvalidPropertyValueException(
            type(),
            "displayName",
            displayName);
    }
    if (displayName != _displayName)
    {   //  Make the change (but no duplication)...
        if (_siblingExists(displayName))
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

QString Workload::description() const
{
    tt3::util::Lock lock(_database->_guard);
    _ensureLive();  //  may throw
    //  We assume database is consistent since last change

    return _description;
}

void Workload::setDescription(
        const QString & description
    )
{
    tt3::util::Lock lock(_database->_guard);
    _ensureLiveAndWritable();   //  may throw
#ifdef Q_DEBUG
    _database->_validate(); //  may throw
#endif

    //  Validate parameters
    if (!_database->_validator->project()->isValidDescription(description))
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
//  tt3::db::api::IWorkload (associations)
auto Workload::contributingActivities(
    ) const -> tt3::db::api::Activities
{
    tt3::util::Lock lock(_database->_guard);
    _ensureLive();  //  may throw
    //  We assume database is consistent since last change

    return tt3::db::api::Activities(_contributingActivities.cbegin(), _contributingActivities.cend());
}

auto Workload::beneficiaries(
    ) const -> tt3::db::api::Beneficiaries
{
    tt3::util::Lock lock(_database->_guard);
    _ensureLive();  //  may throw
    //  We assume database is consistent since last change

    return tt3::db::api::Beneficiaries(_beneficiaries.cbegin(), _beneficiaries.cend());
}

void Workload::setBeneficiaries(
        const tt3::db::api::Beneficiaries & beneficiaries
    )
{
    tt3::util::Lock lock(_database->_guard);
    _ensureLiveAndWritable();   //  may throw
#ifdef Q_DEBUG
    _database->_validate(); //  may throw
#endif

    //  Validate parameters
    if (beneficiaries.contains(nullptr))
    {
        throw tt3::db::api::InvalidPropertyValueException(
            type(),
            "beneficiaries",
            nullptr);
    }

    Beneficiaries xmlBeneficiaries =
        tt3::util::transform(
            beneficiaries,
            [&](auto b)
            {
                Q_ASSERT(b != nullptr); //  should have been caught before!
                auto xmlBeneficiary = dynamic_cast<Beneficiary*>(b);
                if (xmlBeneficiary == nullptr ||
                    xmlBeneficiary->_database != this->_database ||
                    !xmlBeneficiary->_isLive)
                {   //  OOPS!
                    throw tt3::db::api::IncompatibleInstanceException(b->type());
                }
                return xmlBeneficiary;
            });
    if (xmlBeneficiaries != _beneficiaries)
    {   //  Make the changes
        Beneficiaries addedBeneficiaroes = xmlBeneficiaries - _beneficiaries;
        Beneficiaries removedBeneficiaroes = _beneficiaries - xmlBeneficiaries;
        //  link the added beneficiaries...
        for (Beneficiary * xmlBeneficiary : addedBeneficiaroes)
        {
            _beneficiaries.insert(xmlBeneficiary);
            xmlBeneficiary->_workloads.insert(this);
            xmlBeneficiary->addReference();
            this->addReference();
        }
        //  ...un-link the removed beneficiaries...
        for (Beneficiary * xmlBeneficiary : removedBeneficiaroes)
        {
            _beneficiaries.remove(xmlBeneficiary);
            xmlBeneficiary->_workloads.remove(this);
            xmlBeneficiary->removeReference();
            this->removeReference();
        }
        //  ...ensure the changes are saved...
        _database->_markModified();
        //  ...schedule change notifications...
        _database->_changeNotifier.post(
            new tt3::db::api::ObjectModifiedNotification(
                _database, type(), _oid));
        for (Beneficiary * xmlBeneficiary : addedBeneficiaroes + removedBeneficiaroes)
        {
            _database->_changeNotifier.post(
                new tt3::db::api::ObjectModifiedNotification(
                    _database, xmlBeneficiary->type(), xmlBeneficiary->_oid));
        }
        //  ...and we're done
#ifdef Q_DEBUG
        _database->_validate(); //  may throw
#endif
    }
}

void Workload::addBeneficiary(
        tt3::db::api::IBeneficiary * /*beneficiary*/
    )
{
    throw tt3::util::NotImplementedError();
}

void Workload::removeBeneficiary(
        tt3::db::api::IBeneficiary * /*beneficiary*/
    )
{
    throw tt3::util::NotImplementedError();
}

auto Workload::assignedUsers(
    ) const -> tt3::db::api::Users
{
    throw tt3::util::NotImplementedError();
}

void Workload::setAssignedUsers(
        const tt3::db::api::Users & /*users*/
    )
{
    throw tt3::util::NotImplementedError();
}

void Workload::addAssignedUser(
        tt3::db::api::IUser * /*user*/
    )
{
    throw tt3::util::NotImplementedError();
}

void Workload::removeAssignedUser(
        tt3::db::api::IUser * /*user*/
    )
{
    throw tt3::util::NotImplementedError();
}

//////////
//  Implementation helpers
void Workload::_makeDead()
{
    Q_ASSERT(_database->_guard.isLockedByCurrentThread());
    Q_ASSERT(_isLive);

    //  Break associations
    for (Beneficiary * beneficiary : _beneficiaries.values())
    {
        Q_ASSERT(beneficiary->_workloads.contains(this));
        beneficiary->_workloads.remove(this);
        _beneficiaries.remove(beneficiary);
        this->removeReference();
        beneficiary->removeReference();
    }
    _beneficiaries.clear();
    for (User * user : _assignedUsers.values())
    {
        Q_ASSERT(user->_permittedWorkloads.contains(this));
        user->_permittedWorkloads.remove(this);
        _assignedUsers.remove(user);
        this->removeReference();
        user->removeReference();
    }
    _assignedUsers.clear();
    for (Activity * activity : _contributingActivities.values())
    {
        Q_ASSERT(activity->_workload == this);
        activity->_workload = nullptr;
        _contributingActivities.remove(activity);
        this->removeReference();
        activity->removeReference();
    }
    _contributingActivities.clear();

    //  The rest is up to the base class
    Object::_makeDead();
}

//////////
//  Serialization
void Workload::_serializeProperties(
        QDomElement & objectElement
    ) const
{
    Object::_serializeProperties(objectElement);

    objectElement.setAttribute("DisplayName", _displayName);
    objectElement.setAttribute("Description", _description);
}

void Workload::_serializeAggregations(
        QDomElement & objectElement
    ) const
{
    Object::_serializeAggregations(objectElement);
}

void Workload::_serializeAssociations(
        QDomElement & objectElement
    ) const
{
    Object::_serializeAssociations(objectElement);

    _database->_serializeAssociation(
        objectElement,
        "Beneficiaries",
        _beneficiaries);
    _database->_serializeAssociation(
        objectElement,
        "AssignedUsers",
        _assignedUsers);
    _database->_serializeAssociation(
        objectElement,
        "ContributingActivities",
        _contributingActivities);
}

void Workload::_deserializeProperties(
        const QDomElement & objectElement
    )
{
    Object::_deserializeProperties(objectElement);

    _displayName = objectElement.attribute("DisplayName");
    _description = objectElement.attribute("Description");
}

void Workload::_deserializeAggregations(
        const QDomElement & objectElement
    )
{
    Object::_deserializeAggregations(objectElement);
}

void Workload::_deserializeAssociations(
        const QDomElement & objectElement
    )
{
    Object::_deserializeAssociations(objectElement);

    _database->_deserializeAssociation(
        objectElement,
        "Beneficiaries",
        _beneficiaries);
    _database->_deserializeAssociation(
        objectElement,
        "AssignedUsers",
        _assignedUsers);
    _database->_deserializeAssociation(
        objectElement,
        "ContributingActivities",
        _contributingActivities);
}

//////////
//  Validation
void Workload::_validate(
        Objects & validatedObjects
    )
{
    Object::_validate(validatedObjects);

    //  Validate properties
    if (!_database->_validator->workload()->isValidDisplayName(_displayName))
    {   //  OOPS!
        throw tt3::db::api::DatabaseCorruptException(_database->_address);
    }
    if (!_database->_validator->workload()->isValidDescription(_description))
    {   //  OOPS!
        throw tt3::db::api::DatabaseCorruptException(_database->_address);
    }

    //  Validate aggregations

    //  Validate associations
    for (Beneficiary * beneficiary : _beneficiaries)
    {
        if (beneficiary == nullptr ||
            beneficiary->_database != this->_database ||
            !beneficiary->_isLive ||
            !beneficiary->_workloads.contains(this))
        {   //  OOPS!
            throw tt3::db::api::DatabaseCorruptException(_database->_address);
        }
    }
    for (User * user : _assignedUsers)
    {
        if (user == nullptr ||
            user->_database != this->_database ||
            !user->_isLive ||
            !user->_permittedWorkloads.contains(this))
        {   //  OOPS!
            throw tt3::db::api::DatabaseCorruptException(_database->_address);
        }
    }
    for (Activity * activity : _contributingActivities)
    {
        if (activity == nullptr ||
            activity->_database != this->_database ||
            !activity->_isLive ||
            activity->_workload != this)
        {   //  OOPS!
            throw tt3::db::api::DatabaseCorruptException(_database->_address);
        }
    }
}

//  End of tt3-db-xml/Workload.cpp
