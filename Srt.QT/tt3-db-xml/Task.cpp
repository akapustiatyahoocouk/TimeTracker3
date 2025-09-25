//
//  tt3-db-xml/Task.cpp - tt3::db::xml::Task class implementation
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
Task::Task(
        Database * database,
        tt3::db::api::Oid oid
    ) : Activity(database, oid)
{
}

Task::~Task()
{
}

//////////
//  Serialization
void Task::_serializeProperties(
        QDomElement & objectElement
    )
{
    //  Activity properties of a concrete Task
    //  will be serialized via Activity route

    objectElement.setAttribute("RequireCommentOnCompletion", tt3::util::toString(_requireCommentOnCompletion));
    objectElement.setAttribute("Completed", tt3::util::toString(_completed));
}

void Task::_serializeAggregations(
        QDomElement & /*objectElement*/
    )
{
    //  Activity aggregations of a concrete Task
    //  will be serialized via Activity route
}

void Task::_serializeAssociations(
        QDomElement & /*objectElement*/
    )
{
    //  Activity associations of a concrete Task
    //  will be serialized via Activity route
}

void Task::_deserializeProperties(
        const QDomElement & objectElement
    )
{
    //  Activity properties of a concrete Task
    //  will be deserialized via Activity route

    _requireCommentOnCompletion = tt3::util::fromString<bool>(objectElement.attribute("RequireCommentOnCompletion"));
    _completed = tt3::util::fromString<bool>(objectElement.attribute("Completed"));
}

void Task::_deserializeAggregations(
        const QDomElement & /*objectElement*/
    )
{
    //  Activity aggregations of a concrete Task
    //  will be deserialized via Activity route
}

void Task::_deserializeAssociations(
        const QDomElement & /*objectElement*/
    )
{
    //  Activity associations of a concrete Task
    //  will be deserialized via Activity route
}

//////////
//  tt3::db::api::ITask (properties)
bool Task::completed(
    ) const
{
    throw tt3::util::NotImplementedError();
}

void Task::setCompleted(
        bool /*completed*/
    )
{
    throw tt3::util::NotImplementedError();
}

bool Task::requireCommentOnCompletion(
    ) const
{
    throw tt3::util::NotImplementedError();
}

void Task::setRequireCommentOnCompletion(
        bool /*requireCommentOnCompletion*/
    )
{
    throw tt3::util::NotImplementedError();
}

//////////
//  Validation
void Task::_validate(
        Objects & /*validatedObjects*/
    )
{
    //  Activity properties of a concrete Task
    //  will be validated via Activity route

    //  Nothing to validate in Task itself
}

//  End of tt3-db-xml/User.cpp
