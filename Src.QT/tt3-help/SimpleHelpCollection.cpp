//
//  tt3-help/SimpleHelpCollection.cpp - tt3::help::SimpleHelpCollection class implementation
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
#include "tt3-help/API.hpp"
using namespace tt3::help;

//////////
//  Construction/destruction
SimpleHelpCollection::SimpleHelpCollection(
        const QString & name,
        const QString & displayName,
        IContentLoader * contentLoader
    ) : HelpTopic(nullptr),
        HelpCollection(),
        SimpleHelpTopic(nullptr, name, displayName, contentLoader)
{
}

SimpleHelpCollection::~SimpleHelpCollection()
{
}

//////////
//  Serialization
QString SimpleHelpCollection::collectionElementTag() const
{
    return CollectionElementTag;
}

void SimpleHelpCollection::serialize(QDomElement collectionElement)
{
    SimpleHelpTopic::_serialize(collectionElement);
    for (SimpleHelpTopic * topic : children)
    {
        topic->_serialize(collectionElement);
    }
}

//  End of tt3-help/SimpleHelpCollection.cpp
